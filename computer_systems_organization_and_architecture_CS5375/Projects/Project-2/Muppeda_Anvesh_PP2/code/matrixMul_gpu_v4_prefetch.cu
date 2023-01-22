#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

__global__ void gpu_Matrix_Multiplication(double *x, double *y, double *ans, int N) 
{
  //calculates unique thread ID in the block
	int t= (blockDim.x*blockDim.y)*threadIdx.z+(threadIdx.y*blockDim.x)+(threadIdx.x);
	//calculates unique block ID in the grid
	int b= (gridDim.x*gridDim.y)*blockIdx.z+(blockIdx.y*gridDim.x)+(blockIdx.x);
	//block size (this is redundant though)
	int T= blockDim.x*blockDim.y*blockDim.z;
	//grid size (this is redundant though)
	int B= gridDim.x*gridDim.y*gridDim.z;
	
  int AC = (N*N)/(T*B);
  int threadComputation = AC * N;
  /*  printf("Curretly total number of Assigned Cells are: %d\n", AC);
      printf("Each cell in the matrix is assigned to a different thread. 
      Each thread do O(%d) computation.", threadComputation);
	    Assigned cells of different threads does not overlape with 
      each other. And so no need for synchronization.
	 */
	 
    for (int i=b;i<N;i+=B)
    {
      for(int j=t;j<N;j+=T)
      {
        for(int k=0;k<N;k++)
        {
          ans[i*N+j]+=(x[i*N+k]*y[k*N+j]);
        }
      }
	  }
}

//initialize x,y and ans arrays on the GPU
__global__ void init(double *x, double *y, double *ans, int N) 
{
  //Caluculating the row
  int i = blockIdx.y * blockDim.y + threadIdx.y;
  //Calculating the column
  int j = blockIdx.x * blockDim.x + threadIdx.x;
            x[i*N+j] = 5;
            y[i*N+j] = (i==j?1:0);
            ans[i*N+j] = (double)0.000000000000;
}

// ---------------------------------------------------------------------- check
bool check(int N, double *ans)
{
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      if(ans[i*N+j] != 20.0) return false;
    }
  }
  return true;
}

int main(int argc, char const *argv[])
{
    int N= 1<<9;
    int iter = 3;
    clock_t t;

    // allocate memory in host RAM
    double *x, *y, *ans;
    cudaMallocManaged((void **) &x, sizeof(double)*N*N);
    cudaMallocManaged((void **) &y, sizeof(double)*N*N);
    cudaMallocManaged((void **) &ans, sizeof(double)*N*N);

    // Prefetch the data to the GPU
    int device = -1;
    cudaGetDevice(&device);
    cudaMemPrefetchAsync(x, N*N*sizeof(double), device, NULL);
    cudaMemPrefetchAsync(y, N*N*sizeof(double), device, NULL);
    cudaMemPrefetchAsync(ans, N*N*sizeof(double), device, NULL);

    //Defining the number of threads per block
    int THREADS = 8;

    //Defining the number of blocks to the grid
    int BLOCKS = N / THREADS;

    //Calculating the number of threads and blocks using the 2D dim3
    dim3 threads(THREADS, THREADS);
    dim3 blocks(BLOCKS, BLOCKS);
   
    // Launch initilization kernel with the 4095 blocks and 64 threads
    init<<<blocks, threads>>>(x, y, ans, N);
    
    // Launch matrix multiplcation kernel
    double avg = 0;
    std::cout<<"Starting GPU computation"<<std::endl;
    for(int i = 0; i <= iter; i++) {
        t = clock();
        //Launch the kernel with the 256 threads and 256 blocks which is optimized
        gpu_Matrix_Multiplication<<<dim3(2,2,64), dim3(2,2,64)>>>(x, y, ans, N);
        t = clock() - t;
        if(i) avg += t;  //we will ignore the first run
        // printf ("It took CPU-%d %f ms.\n",i,(((double)t)/CLOCKS_PER_SEC)*1000);
    }

    //Calculating the avg time over the 3 iterations
    avg /= iter;
    avg /= CLOCKS_PER_SEC;
    avg *= 1000;
    //Priting the Average value i.e. avg time to compute GPU kernel
    printf("It took %lf ms on avg.\n", avg);

    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    // validate results computed by GPU
    if(check(N,ans)) std::cout<<"RUN OK."<<std::endl;
        else std::cout<<"RUN NOT OK."<<std::endl;

    // free memory
    cudaFreeHost(x);
    cudaFreeHost(y);
    cudaFreeHost(ans);

    return 0;
}
/* EOF */
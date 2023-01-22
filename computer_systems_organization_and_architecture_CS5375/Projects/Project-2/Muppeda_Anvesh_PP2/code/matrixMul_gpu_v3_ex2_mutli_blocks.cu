#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

__global__ void gpu_Matrix_Multiplication(double *x, double *y, double *ans, int N) 
{
  //calculates current thread ID with the help of block and grid dim
	int t = (blockDim.x*blockDim.y)*threadIdx.z+(threadIdx.y*blockDim.x)+(threadIdx.x);
	//calculates current block id with the help of grid and block id
	int b = (gridDim.x*gridDim.y)*blockIdx.z+(blockIdx.y*gridDim.x)+(blockIdx.x);
	//block size (Number of threads per block)
	int T = blockDim.x*blockDim.y*blockDim.z;
	//grid size (Number of blocks per grid)
	int B = gridDim.x*gridDim.y*gridDim.z;

  //Calculating the AC value with the above values
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
          //Calculating the target matrix
          ans[i*N+j]+=(x[i*N+k]*y[k*N+j]);
        }
      }
	  }
}

// ---------------------------------------------------------------------- check function on the host
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
    // size of matrix
    int N = 1<<9; // binary left-shift: 1 * 2^9 = 512
    int iter = 3;
    clock_t t;

    // Martices
    double *x, *y, *ans;
    
    // Allocate Unified Memory - accessible from both CPU and GPU
    cudaMallocManaged(&x, N*N*sizeof(double));
    cudaMallocManaged(&y, N*N*sizeof(double));
    cudaMallocManaged(&ans, N*N*sizeof(double));

    //Initialize x,y and ans arrays on the host
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            x[i*N+j] = 5;
            y[i*N+j] = (i==j?1:0);
            ans[i*N+j] = (double)0.000000000000;
        }
    }
    
    double avg = 0;
    std::cout<<"Starting GPU computation"<<std::endl;
    for(int i = 0; i <= iter; i++) {
        t = clock();
        //Launch kernel on GPU with the 1024 blocks and 1024 threads
        gpu_Matrix_Multiplication<<<dim3(64,4,4), dim3(64,4,4)>>>(x, y, ans, N);
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
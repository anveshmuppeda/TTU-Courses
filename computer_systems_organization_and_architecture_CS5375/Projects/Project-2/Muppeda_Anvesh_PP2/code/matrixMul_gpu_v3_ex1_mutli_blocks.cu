#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

//Computing the matrix multiplication
__global__ void gpu_Matrix_Multiplication(double *x, double *y, double *ans, int N) 
{
  //Calculate the index value based on th threadId and blockId values(here we are passing multiple blocks along with the multiple threads)
  int index = blockIdx.x * N + threadIdx.x;
  //Iterate over row, and column of the target matrices
  for (int k = 0; k < N; k++) {
    //Accumulate results for a single element
    ans[ index ] += x[ blockIdx.x * N + k] * y[k * N + threadIdx.y ];
  }
}

// ---------------------------------------------------------------------- check function on host
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

    // initialize x,y and ans arrays on the host
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            x[i*N+j] = 5;
            y[i*N+j] = (i==j?1:0);
            ans[i*N+j] = (double)0.000000000000;
        }
    }

    // Run kernel on N*N elements on the GPU
    //Defining the Block Size
    int blockSize = 512;
    //Defining the number of blocks
    int numBlocks = (N * N) / blockSize;

    double avg = 0;
    std::cout<<"Starting GPU computation"<<std::endl;
    for(int i = 0; i <= iter; i++) {
        t = clock();
        // Launch kernel with blocks and blocksize
        gpu_Matrix_Multiplication<<<numBlocks, blockSize>>>(x, y, ans, N);
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
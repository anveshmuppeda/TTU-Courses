/*
 * _MATRIXMUL_GPU_CU_
 *
 * 2022 Mert SIDE
 *
 * CS5375 Computer Systems Organization and Architecture 
 * Guest Lecture: GPU Programming
 *
 * Multiplying two matrices on the GPU
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------ gpu_Matrix_Multiplication
__global__
void gpu_Matrix_Multiplication(int N, double *x, double *y, double *ans)
{
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      for(int k = 0; k < N; k++) {
        ans[i*N+j] += (x[i*N+k] * y[k*N+j]);
      }
    }
  }
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

// ----------------------------------------------------------------------- MAIN
int main(void)
{
  // size of matrix
  int N = 1<<9; // binary left-shift: 1 * 2^9 = 512
  printf("Size of matrix (N) is %d by %d.\n", N, N);
  int iter = 3;
  clock_t t;
  
  // Martices
  double *x, *y, *ans;

  // Allocate Unified Memory - accessible from both CPU and GPU
  cudaMallocManaged(&x, N*N*sizeof(double));
  cudaMallocManaged(&y, N*N*sizeof(double));
  cudaMallocManaged(&ans, N*N*sizeof(double));
  
  // ..........................................................................
  // initialize x,y and ans arrays on the host
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      x[i*N+j] = 5;
      y[i*N+j] = (i==j?1:0);
      ans[i*N+j] = (double)0.000000000000;
    }
  }

  // ..........................................................................
  double avg=0;
  std::cout<<"Starting unoptimized GPU computation"<<std::endl;
  // Run kernel on GPU
  for(int i = 0; i <= iter; i++) {
    t = clock();
    gpu_Matrix_Multiplication<<<1,1>>>(N, x, y,ans);
    cudaDeviceSynchronize();
    t = clock() - t;
    if(i) avg += t; //we will ignore the first run
    // printf ("It took GPU-%d %f ms.\n",i,(((double)t)/CLOCKS_PER_SEC)*1000);
  }

  //Calculating the avg time over the 3 iterations
  avg /= iter;
  avg /= CLOCKS_PER_SEC;
  avg *= 1000;
  //Priting the Average value i.e. avg time to compute GPU kernel
  printf("It took %lf ms on avg.\n", avg);

  // validate results computed by GPU
  if(check(N,ans)) std::cout<<"RUN OK."<<std::endl;
    else std::cout<<"RUN NOT OK."<<std::endl;

  // ..........................................................................s
  // Free memory
  cudaFree(x);
  cudaFree(y);
  cudaFree(ans);

  return 0;
}
/* EOF */
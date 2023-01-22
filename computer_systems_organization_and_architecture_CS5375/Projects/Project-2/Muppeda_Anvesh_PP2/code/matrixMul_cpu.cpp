/*
 * _MATRIXMUL_CPU_CPP_
 *
 * 2022 Mert SIDE
 *
 * CS5375 Computer Systems Organization and Architecture 
 * Guest Lecture: GPU Programming
 *
 * Multiplying two matrices on the CPU
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------ CPUmatmul
void CPUmatmul(int N, double *x, double *y, double *ans)
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

  // Allocate Memory - accessible from CPU
  double *x   = new double[N*N];
  double *y   = new double[N*N];
  double *ans = new double[N*N];

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
  double avg = 0;
  std::cout<<"Starting CPU computation"<<std::endl;
  for(int i = 0; i <= iter; i++) {
    t = clock();
    CPUmatmul(N, x, y,ans);
    t = clock() - t;
    if(i) avg += t;  //we will ignore the first run
    // printf ("It took CPU-%d %f ms.\n",i,(((double)t)/CLOCKS_PER_SEC)*1000);
  }

  avg /= iter;
  avg /= CLOCKS_PER_SEC;
  avg *= 1000;
  printf("It took %lf ms on avg.\n", avg);
  if(check(N,ans)) std::cout<<"RUN OK."<<std::endl;
  else std::cout<<"RUN NOT OK."<<std::endl;

  // ..........................................................................
  // Free memory
  delete [] x;
  delete [] y;
  delete [] ans;

  return 0;
}
/* EOF */

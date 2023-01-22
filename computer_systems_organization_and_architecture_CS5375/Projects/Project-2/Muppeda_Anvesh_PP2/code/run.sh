#!/bin/bash

echo "================================================Running all scripts one by one================================================"

echo "================================================Running CPU version================================================"
./matrixMul_cpu.exe

echo "================================================Running version-1(Unoptimized version)================================================"
nvprof ./matrixMul_gpu_v1.exe

echo "================================================Running version-2(Multi Thread version)================================================"
nvprof ./matrixMul_gpu_v2_multi_thread.exe

echo "================================================Running version-3(Multi Block version example 1)================================================"
nvprof ./matrixMul_gpu_v3_ex1_mutli_blocks.exe

echo "================================================Running version-3(Multi Block version example 2)================================================"
nvprof ./matrixMul_gpu_v3_ex2_mutli_blocks.exe

echo "================================================Running version-4(Optimized version)================================================"
nvprof ./matrixMul_gpu_v4_optimize.exe

echo "================================================Running version-4(Optimized version with prefetch)================================================"
nvprof ./matrixMul_gpu_v4_prefetch.exe

echo "================================================All versions are executed================================================"
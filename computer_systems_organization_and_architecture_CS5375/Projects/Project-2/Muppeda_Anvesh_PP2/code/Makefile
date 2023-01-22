all: app0 app1 app2 app3 app4 app5 app6

app0:
	g++ matrixMul_cpu.cpp -o matrixMul_cpu.exe

app1:
	nvcc matrixMul_gpu_v1.cu -o matrixMul_gpu_v1.exe

app2:
	nvcc matrixMul_gpu_v2_multi_thread.cu -o matrixMul_gpu_v2_multi_thread.exe

app3:
	nvcc matrixMul_gpu_v3_ex1_mutli_blocks.cu -o matrixMul_gpu_v3_ex1_mutli_blocks.exe

app4:
	nvcc matrixMul_gpu_v3_ex2_mutli_blocks.cu -o matrixMul_gpu_v3_ex2_mutli_blocks.exe

app5:
	nvcc matrixMul_gpu_v4_optimize.cu -o matrixMul_gpu_v4_optimize.exe

app6:
	nvcc matrixMul_gpu_v4_prefetch.cu -o matrixMul_gpu_v4_prefetch.exe
 
clean:
	rm -rf *.exe

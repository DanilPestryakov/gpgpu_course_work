
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cooperative_groups.h"
#include <device_functions.h>
#include <cuda_device_runtime_api.h>

#include <stdio.h>

#include<iostream>
#include <cuda.h>

#include <curand.h>
#include <curand_kernel.h>

#include "kernel.cuh"


//#include "ParticleSystem.h"
#include "Particle.h"
#include "Functions.h"
#include <algorithm>


__inline__ __device__ int warpReduceSum(int val)
{
	for (int offset = warpSize / 2; offset > 0; offset /= 2)
		val += __shfl_down_sync(warpSize - 1, val, offset);

	return val;
}

__inline__ __device__ int blockReduceSum(int val)
{
	static __shared__ int shared[32];
	int lane = threadIdx.x % warpSize;
	int wid = threadIdx.x / warpSize;

	val = warpReduceSum(val);

	// write reduced value to shared memory
	if (lane == 0)
		shared[wid] = val;

	__syncthreads();

	// ensure we only grab a value from shared memory 
	// if that warp existed
	val = (threadIdx.x < blockDim.x / warpSize) ? shared[lane] : int(0);

	if (wid == 0)
		val = warpReduceSum(val);

	return val;
}

__global__ void deviceReduceKernel(int* in, int* out, int n)
{
	int sum = 0;

	//reduce multiple elements per thread
	for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < n; i += blockDim.x * gridDim.x)
		sum += in[i];

	sum = blockReduceSum(sum);

	if (threadIdx.x == 0)
		out[blockIdx.x] = sum;
}

void deviceReduce(int* in, int* out, int n)
{
	deviceReduceKernel << <BLOCKS_NUMBER, THREADS_NUMBER >> > (in, out, n);
	deviceReduceKernel << <1, THREADS_NUMBER >> > (out, out, BLOCKS_NUMBER);
}

// Kernel Definition
__global__ void iter(Particle* p1, Particle* p2, Geometry g, int n)
{
	double NewXCoord, NewYCoord;
	double SourceCoordX_1 = 300;
	double SourceCoordY_1 = 100;
	double SourceCoordX_2 = 500;
	double SourceCoordY_2 = 100;
	double BasketLevel = 400;
	double BasketBegin = 350;
	double BasketWidth = 100;
	double Life = 10000;
	double dt = 0.5;

	ParticleParams params1;
	ParticleParams params2;
	int b;

	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < n) {
		
		params1 = p1[i].GetParams();
		NewXCoord = params1.x + params1.Vx * dt;
		NewYCoord = params1.y + params1.Vy * dt;

		p1[i].UpdateParticle(NewXCoord, NewYCoord, g, dt);
		if (params1.Life <= 0) {	
			p1[i].UpdateLifeStatus(SourceCoordX_1, SourceCoordY_1, 0.1, 0.1, 0, 1, 0, Life);
		}

		if (params1.y > BasketLevel) {
			p1[i].UpdateLifeStatus(SourceCoordX_1, SourceCoordY_1, 0.1, 0.1, 0, 1, 0, Life);
		}

		if (params1.y >= BasketLevel && params1.x >= BasketBegin && params1.x <= BasketBegin + BasketWidth) {
			b = params1.InBasket + 1;
			p1[i].SetInBasket(b);
		}

		params2 = p2[i].GetParams();
		NewXCoord = params2.x + params2.Vx * dt;
		NewYCoord = params2.y + params2.Vy * dt;

		p2[i].UpdateParticle(NewXCoord, NewYCoord, g, dt);

		if (params2.Life <= 0) {
			p2[i].UpdateLifeStatus(SourceCoordX_2, SourceCoordY_2, 0.1, 0.1, 0, 0, 1, Life);
		}

		if (params2.y > BasketLevel) {
			p2[i].UpdateLifeStatus(SourceCoordX_2, SourceCoordY_2, 0.1, 0.1, 0, 0, 1, Life);
		}

		if (params2.y > BasketLevel && params2.x >= BasketBegin && params2.x <= BasketBegin + BasketWidth) {
			b = params2.InBasket + 1;
			p2[i].SetInBasket(b);
		}

	}

	__syncthreads();
}

__global__ void iter2(Particle* p1, Particle* p2, int n) {

	double tempVx, tempVy;
	double ParticlesDist;
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int j = blockDim.y * blockIdx.y + threadIdx.y;

	// Checking particle-particle interaction

	if (i >= n || j >= n)
		return;

	ParticlesDist = RDistance(p1[i].GetParams().x, p2[j].GetParams().x, p1[i].GetParams().y, p2[j].GetParams().y);

	if (ParticlesDist <= 14.0) {

		tempVx = p1[i].GetParams().Vx;
		tempVy = p1[i].GetParams().Vy;

		p1[i].SetVelocity(p2[j].GetParams().Vx, p2[j].GetParams().Vy);
		p2[j].SetVelocity(tempVx, tempVy);

	}

		__syncthreads();
	
}

void Calc(Particle* h_a, Particle* h_b, Geometry g, int n) {


	// Allocate host memory
	// Initialize host array

	// Allocate arrays in Device memory
	Particle* d_a;
	Particle* d_b;

	size_t size= BLOCKS_NUMBER*THREADS_NUMBER*sizeof(Particle);

	cudaMalloc(&d_a, size);
	cudaMalloc(&d_b, size);

	// Copy memory from Host to Device
	cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

	// Block and Grid dimentions
	
	iter<<<BLOCKS_NUMBER,THREADS_NUMBER>>>(d_a, d_b, g, n);
	// Launch Kernel

	// Some kind of synchronization
	cudaDeviceSynchronize();

	cudaMemcpy(h_a, d_a, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_b, d_b, size, cudaMemcpyDeviceToHost);

	cudaFree(d_a);
	cudaFree(d_b);
}

void Calc2(Particle* h_a, Particle* h_b, int n) {

	// Allocate host memory
// Initialize host array

// Allocate arrays in Device memory
	Particle* d_a;
	Particle* d_b;

	size_t size = BLOCKS_NUMBER * THREADS_NUMBER * sizeof(Particle);

	cudaMalloc(&d_a, size);
	cudaMalloc(&d_b, size);

	// Copy memory from Host to Device
	cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

	// Block and Grid dimentions

	dim3 block(BLOCKS_NUMBER, BLOCKS_NUMBER);
	dim3 grid(THREADS_NUMBER, THREADS_NUMBER);
	iter2 << <grid, block >> > (d_a, d_b, n);
	// Launch Kernel

	// Some kind of synchronization
	cudaDeviceSynchronize();

	cudaMemcpy(h_a, d_a, size, cudaMemcpyDeviceToHost);
	cudaMemcpy(h_b, d_b, size, cudaMemcpyDeviceToHost);

	cudaFree(d_a);
	cudaFree(d_b);
}

int Calc3(Particle* h_a, Particle* h_b, int n) {

	// Allocate host memory
// Initialize host array

// Allocate arrays in Device memory
	int counter = 0;
	int* a;
	int* b;

	// allocate memory
	cudaMallocManaged(&a, 2 * n * sizeof(int));
	cudaMallocManaged(&b, 2 * n * sizeof(int) / THREADS_NUMBER);	// we need space for every block, ie n/512 elements

		// fill it with data
	for (int i = 0; i < n; i++) {
		a[i] = h_a[i].GetInBasket();
		a[i + n] = h_b[i].GetInBasket();
	}

	deviceReduce(a, b, 2*n);

	cudaDeviceSynchronize();

	counter = b[0];

	cudaFree(a);
	cudaFree(b);

	return counter;
}

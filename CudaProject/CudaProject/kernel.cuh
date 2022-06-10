#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

#include<iostream>
#include <cuda.h>

#include <curand.h>
#include <curand_kernel.h>

#define THREADS_NUMBER 256
#define BLOCKS_NUMBER 20


//void Particle::Calc(Particle* h_a, Geometry g, int n);

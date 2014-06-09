// CUDA functions/kernels for computing the 
// TB-SMA energy/force

#include <iostream>
using std::cout;
using std::endl;

#include "TBSMA_GPU_Compute.cuh"

#define BLOCK_SIZE 64 // this block size is optimized for larger systems

__global__ void TBSMA_kernel_1( float * xpos, float * ypos, float * zpos, float * rho, 
                                           int natoms, float * box_len, float * inv_box_len )
{

	float param_ro = 2.88426;
	float param_2q = 2.0 * 4.036;
	float r_cutsq = 5.8 * 5.8;
	float energy_band_term = 0.0;
	
	volatile int idx = blockIdx.x * blockDim.x + threadIdx.x;
	
	if (idx >= natoms) return;
	
	// read in particle positions from device global memory
	// textured reads may improve performance here
	float xi = xpos[ idx ];
	float yi = ypos[ idx ];
	float zi = zpos[ idx ];
	float dx,dy,dz,rsq;
	
	// loop over all atoms, not using Newton's 2nd law here 
	// because that requires communication between threads, it's
	// more efficient to just re-calculate the energy between
	// two atoms.
	//
	// A neighborlist would be more efficient here but for 
	// simplicity I am not using one
	//
	for (int i = 0; i < natoms ; i++)
	{
		
		if (idx != i) // don't count self-interaction
		{
		
			dx = xi - xpos[ i ];
			dy = yi - ypos[ i ];
			dz = zi - zpos[ i ];
			
			// periodic boundary conditions
			dx -= box_len[0] * rintf( dx * inv_box_len[0] );
			dy -= box_len[1] * rintf( dy * inv_box_len[1] );
			dz -= box_len[2] * rintf( dz * inv_box_len[2] );
		
			rsq = dx*dx + dy*dy + dz*dz;
			
			if (rsq < r_cutsq)
			{
			
				float r = sqrt( rsq );
				float r_modified = 1.0 - r / param_ro;
				energy_band_term += exp( param_2q * r_modified );
			
			}
			
		
		}
	
	}
	
	rho[ idx ] = energy_band_term;

}

__global__ void TBSMA_kernel_2( float * xpos, float * ypos, float * zpos, float * rho, 
                                           int natoms, float * box_len, float * inv_box_len,
										   float * energy_device, float * force_device )
{

	float param_P = 10.229;
	float param_q = 4.036;
	float param_ro = 2.88426;
	float param_2q = 2.0 * 4.036;
	float r_cutsq = 5.8 * 5.8;
	
	float param_A = 0.2061;
	float param_B = 1.790;
	
	float param_BQinvro = param_B * param_q / param_ro;
	float param_APinvro = 2.0 * param_A * param_P / param_ro;
	
	volatile int idx = blockIdx.x * blockDim.x + threadIdx.x;
	energy_device[ idx ] = 0.0;
	force_device[ idx ] = 0.0;
	
	if (idx >= natoms) return;
	
	// read in particle positions from device global memory
	// textured reads may improve performance here
	float xi = xpos[ idx ];
	float yi = ypos[ idx ];
	float zi = zpos[ idx ];
	float dx,dy,dz,rsq;
	float fxi = 0.0;
	float fyi = 0.0;
	float fzi = 0.0;
	float m_pe = 0.0;
	float pairForce = 0.0;
	float pair_eng = 0.0;
	
	// rho from previous kernel
	float rho_i = rho[ idx ];
	float sqrt_rho_i = sqrtf( rho_i );
	float inv_sqrt_rho_i = 1.0 / sqrt_rho_i;
	
	// loop over all atoms
	for (int i = 0; i < natoms ; i++)
	{
		
		if (idx != i) // don't cout self-interaction
		{
		
			dx = xi - xpos[ i ];
			dy = yi - ypos[ i ];
			dz = zi - zpos[ i ];
			
			// periodic boundary conditions
			dx -= box_len[0] * rint( dx * inv_box_len[0] );
			dy -= box_len[1] * rint( dy * inv_box_len[1] );
			dz -= box_len[2] * rint( dz * inv_box_len[2] );
		
			rsq = dx*dx + dy*dy + dz*dz;
			
			if (rsq < r_cutsq)
			{
			
				float r = sqrt( rsq );
				float rinv = 1.0 / r;
				float r_modified = 1.0 - r / param_ro;
				
				float energy_repulsive = exp( param_P * r_modified );
				float energy_temp = exp( param_2q * r_modified );
				
				float rho_j = rho[ i ];
				float inv_sqrt_rho_j = 1.0 / sqrt( rho_j );
				
				pair_eng = energy_repulsive * param_A; // repulsive, pairwise energy
				pairForce = -param_BQinvro * energy_temp * (inv_sqrt_rho_i + inv_sqrt_rho_j) + energy_repulsive * param_APinvro;
                pairForce *= rinv;
				
				fxi += dx * pairForce ;
				fyi += dy * pairForce ;
				fzi += dz * pairForce ;
				m_pe += pair_eng;

			
			}
			
		
		}
	
	}
	
	energy_device[ idx ] = m_pe - param_B * sqrt_rho_i; // PE of this thread's atom
	force_device[ idx ] = sqrt( fxi*fxi + fyi*fyi + fzi*fzi ); // Force acting on this thread's atom

}

__global__ void Reduce_Energy( float * energy, float * force )
{

	__shared__ float Es[BLOCK_SIZE];
	__shared__ float Fs[BLOCK_SIZE];

	unsigned int tid = threadIdx.x;
	unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
	
	// load global data into shared memory
	Es[ tid ] = energy[ i ];
	Fs[ tid ] = force[ i ];
	
	__syncthreads(); // wait for everything to load
	
	for (unsigned int s=blockDim.x/2; s>0; s>>=1) {
		if (tid < s) {
		
			Es[ tid ] += Es[ tid + s ];
			Fs[ tid ] += Fs[ tid + s ];
		
		}
		__syncthreads();  // wait for all threads to finish
	
	}
	
	// reduce in shared memory (naive version)
	//for (unsigned int s=1; s < blockDim.x; s *= 2) {
	
	//	if ( tid % (2*s) == 0 ) {
	//		Es[ tid ] += Es[ tid + s ];
	//		Fs[ tid ] += Fs[ tid + s ];
	//	}
	//	__syncthreads(); 
	
	//}
	
	// copy results back to the energy and force arrays,
	// I'll sum up the results from each block on the CPU
	energy[ i ] = Es[ tid ];
	force[ i ] = Fs[ tid ];
	

}

// this function calls the CUDA kernels that calculate the TB-SMA energy/force
void gpu_compute_TBSMA( Atoms * host_atoms , Device_Atoms * device_atoms, Device_Box * device_box, float * host_energy, 
                        float * host_force, float ** device_energy, float ** device_force, bool print_step )
{

    //
	// (1) copy atom positions to device
	// (2) compute the energy/force on the device
	// (3) copy the energy/force back to the CPU
	//
	
	
	float * h_xtmp = host_atoms->getCoords(1);
	float * h_ytmp = host_atoms->getCoords(2);
	float * h_ztmp = host_atoms->getCoords(3);
	float ** d_xtmp = device_atoms->getDevPtr(1);
	float ** d_ytmp = device_atoms->getDevPtr(2);
	float ** d_ztmp = device_atoms->getDevPtr(3);
	float ** rho = device_atoms->getDevPtr(4);
	float ** d_box_len = device_box->getDevPtr(1);
	float ** d_inv_box_len = device_box->getDevPtr(2);
	
	int n = host_atoms->printNatoms();
	
	size_t size = sizeof(float)*n;
	
	// copy atom coordinates to the GPU every time step
	//cudaError_t result;
	//result = cudaMemcpy(*d_xtmp,h_xtmp,size,cudaMemcpyHostToDevice);
	cudaMemcpy(*d_xtmp,h_xtmp,size,cudaMemcpyHostToDevice);
	cudaMemcpy(*d_ytmp,h_ytmp,size,cudaMemcpyHostToDevice);
	cudaMemcpy(*d_ztmp,h_ztmp,size,cudaMemcpyHostToDevice);
	
	int blocks = (int)ceil((float)n / (float)BLOCK_SIZE);

    dim3 grid(blocks, 1, 1);
    dim3 threads(BLOCK_SIZE, 1, 1);
	
	// call the first kernel for calculating the electron density
	TBSMA_kernel_1<<< grid, threads>>>( *d_xtmp, *d_ytmp, *d_ztmp, *rho, n, *d_box_len, *d_inv_box_len );
	
	// call the second kernel for calculating the energy/force
	TBSMA_kernel_2<<< grid, threads>>>( *d_xtmp, *d_ytmp, *d_ztmp, *rho, n, *d_box_len, *d_inv_box_len,
	                                    *device_energy, *device_force );
										
	
	// if energy/force need to be output this timestep,
	// copy resulting energy/force to the CPU
	if (print_step) {
	
		// get the total energy/force by summing up the energy/force
		// acting on individual atoms									
		Reduce_Energy<<< grid, threads>>>( *device_energy, *device_force );
		
		// Copy results to CPU
		cudaMemcpy(host_energy,*device_energy,size,cudaMemcpyDeviceToHost);
		cudaMemcpy(host_force,*device_force,size,cudaMemcpyDeviceToHost);
		
		for (int j=1; j<blocks; j++) { // add up the block sums
			host_energy[0] += host_energy[ j*BLOCK_SIZE ];
			host_force[0] += host_force[ j*BLOCK_SIZE ];
		}
	
	}

}

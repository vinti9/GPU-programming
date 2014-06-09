// function definitions for creating space on device for atoms

#include "device_space.cuh"

void make_device_space( Device_Atoms * dev_atoms, Device_Box * dev_box, Box host_box, int natoms )
{

	// make room for atoms on device, the positions will be copied before each timestep
	size_t size = sizeof(float)*natoms;
	cudaMalloc( dev_atoms->getDevPtr(1) , size );
	cudaMalloc( dev_atoms->getDevPtr(2) , size );
	cudaMalloc( dev_atoms->getDevPtr(3) , size );
	cudaMalloc( dev_atoms->getDevPtr(4) , size );
	
	// make room for box dimensions on device
	cudaMalloc( dev_box->getDevPtr(1) , 3*sizeof(float) );
	cudaMalloc( dev_box->getDevPtr(2) , 3*sizeof(float) );
	
	// copy the box info to the device (this only needs to be done once)
	float ** d_box_len = dev_box->getDevPtr(1);
	float ** d_inv_box_len = dev_box->getDevPtr(2);
	float * h_box_len = host_box.getLength(1);
	float * h_box_len_inv = host_box.getLength(2);
	cudaMemcpy(*d_box_len,h_box_len,3*sizeof(float),cudaMemcpyHostToDevice);
	cudaMemcpy(*d_inv_box_len,h_box_len_inv,3*sizeof(float),cudaMemcpyHostToDevice);

}

void clear_device_space( float ** xtmp, float ** ytmp, float ** ztmp, float ** rho, 
                         float ** Box_len, float ** Box_len_inv )
{

    // free up all the GPU memory
	cudaFree( *xtmp );
	cudaFree( *ytmp );
	cudaFree( *ztmp );
	cudaFree( *rho );
	cudaFree( *Box_len );
	cudaFree( *Box_len_inv );

}

void make_space_compute( float ** denergy, float ** dforce, int n )
{

	// make room on the GPU for the energy and force associated with each atom 
	size_t size = sizeof(float)*n;
	cudaMalloc( denergy , size ); // total potential energy
	cudaMalloc( dforce , size ); // 3d force
	
	//cudaError_t result;
	//result = cudaMalloc( dforce , 3*sizeof(float) ); // 3d force
	//cout << "comments from cudaMalloc: " << cudaGetErrorString(result) << endl;


} 

void clear_space_compute( float ** denergy, float ** dforce)
{

	// free space on GPU
	cudaFree( *denergy );
	cudaFree( *dforce );

}

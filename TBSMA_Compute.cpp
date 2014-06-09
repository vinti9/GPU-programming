// member functions for TBSMA_Compute Class

#include <iostream>
using std::cout;
using std::endl;

#include "TBSMA_Compute.hpp"
#include "TBSMA_GPU_Compute.cuh"
#include "device_space.cuh"

TBSMA_Compute::TBSMA_Compute( int n )
{
	
	h_energy = new float[ n ];
	h_force = new float[ n ];
	
	// initialize all the data to zero
	for (int i=0; i<n ; i++) {
		h_energy[i] = 0.0;
		h_force[i] = 0.0;
	}
	ncnt = 0;
	
	// assign pointers
	d_energy_ptr = &d_energy;
	d_force_ptr = &d_force;
	// clear space on device
	make_space_compute( d_energy_ptr , d_force_ptr, n );

}

TBSMA_Compute::~TBSMA_Compute()
{
	
	// clean up
	delete [] h_energy;
	delete [] h_force;
	clear_space_compute( d_energy_ptr , d_force_ptr );
	
}

void TBSMA_Compute::compute( Atoms * h_atoms , Device_Atoms * d_atoms, Device_Box * d_box, bool print_step )
{
	
	// this is where I call the device to calculate the energy
	// and force
	gpu_compute_TBSMA( h_atoms , d_atoms, d_box, h_energy, h_force, d_energy_ptr, d_force_ptr, print_step );

}

string TBSMA_Compute::getName() const
{
	
	return "TB-SMA Energy/Force";
	
}

void TBSMA_Compute::print_message() const
{
	
	cout << "Computing the energy/force via the TB-SMA Potential..." << endl;
	
}

void TBSMA_Compute::incrementCnt()
{
	
	ncnt++; // number of times the compute has been called
	
}

int TBSMA_Compute::printCnt() const
{
	
	return ncnt;

}

float TBSMA_Compute::printEnergy() const
{
	
	return h_energy[0]; // total energy reduced into the first element
	
}

float TBSMA_Compute::printForce() const
{
	
	return h_force[0];  // total force reduced into the first element
	
}

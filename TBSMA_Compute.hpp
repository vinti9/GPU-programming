// definition for TBSMA_Compute class

#ifndef TBSMA_COMPUTE_HPP
#define TBSMA_COMPUTE_HPP

#include "Compute.hpp"

class TBSMA_Compute : public Compute {
	
public:
	
	TBSMA_Compute( int );
	~TBSMA_Compute();
	virtual void compute( Atoms * , Device_Atoms *, Device_Box *, bool ); // computes the TBSMA energy, calls CUDA kernels
	virtual string getName() const; // returns name of Computer
	virtual void print_message() const; // prints a message related to the compute function
	void incrementCnt();
	int printCnt() const;
	float printEnergy() const;
	float printForce() const;
	
private:
	
	float * h_energy; // energy on host
	float * h_force; // force on host
	float * d_energy; // energy on device
	float * d_force; // force on device
	float ** d_energy_ptr; // device pointer, aims at d_energy
	float ** d_force_ptr; // device pointer, aims at d_force
	int ncnt; //counter
	
};

#endif

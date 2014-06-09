// member function definitions for object Device_Atoms

#include <iostream>
using std::cout;
using std::endl;

#include "Device_Atoms.hpp"

Device_Atoms::Device_Atoms()
{

}

Device_Atoms::~Device_Atoms()
{
	
}

// return device pointer for allocating memory,
// copying between host and device, etc.
float ** Device_Atoms::getDevPtr( int dimension )
{
	
	if (dimension == 1) return xx_dev_ptr;
	if (dimension == 2) return yy_dev_ptr;
	if (dimension == 3) return zz_dev_ptr;
	if (dimension == 4) return rho_dev_ptr;
		
	if (dimension < 1 || dimension > 4) {
		cout << "dimension must be between 1 and 4!" << endl;
		exit(1);
		return NULL;
	}
		
}

void Device_Atoms::setupDevicePtrs()
{
	
	xx_dev_ptr = &xx;
	yy_dev_ptr = &yy;
	zz_dev_ptr = &zz;
	rho_dev_ptr = &rho;
	
}

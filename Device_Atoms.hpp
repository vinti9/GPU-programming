// class definition for class Atoms

#ifndef DEVICE_ATOMS_HPP
#define DEVICE_ATOMS_HPP

class Device_Atoms {
	
public:
	Device_Atoms(); // default constructor
	~Device_Atoms(); //destructor
	float ** getDevPtr( int ); // returns device pointer
	void setupDevicePtrs(); // setup device pointers
	
private:
	float * xx; // array of x particle positions
	float * yy; // array of y particle positions
	float * zz; // array of z particle positions
	float * rho; // electron density
	float ** xx_dev_ptr; // device pointer, points to *xx
	float ** yy_dev_ptr; // device pointer, points to *yy
	float ** zz_dev_ptr; // device pointer, points to *zz
	float ** rho_dev_ptr; // device pointer, points to *rho

};

#endif

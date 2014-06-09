// Member function definitions for class Device_Box

#include <iostream>
using std::cout;
using std::endl;

#include "Device_Box.hpp"

Device_Box::Device_Box() // Constructor 
{
	

	//Lbox = NULL;
	//Lbox_inv = NULL;
	
	//cout << "Box constructed" << endl;
	
}

Device_Box::~Device_Box()
{
	
	//cout << "Box destroyed" << endl;
	
}

float ** Device_Box::getDevPtr( int dimension ) 
{
	
	if (dimension == 1) return Lbox_ptr;
	if (dimension == 2) return Lbox_inv_ptr;
	
	if (dimension < 1 || dimension > 2) {
		cout << "dimension must be between 1 and 2!" << endl;
		exit(1);
		return NULL;
	}
	
}

void Device_Box::setupDevicePtrs()
{
	
	Lbox_ptr = &Lbox;
	Lbox_inv_ptr = &Lbox_inv;
	
}

// Member function definitions for class Box

#include <iostream>
using std::cout;
using std::endl;

#include "Box.hpp"

Box::Box() // Constructor 
{
	
	xbox[0] = xbox[1] = 0.00;
	ybox[0] = ybox[1] = 0.00;
	zbox[0] = zbox[1] = 0.00;
	Lbox[0] = Lbox[1] = Lbox[2] = 0.00;
	Lbox_inv[0] = Lbox_inv[1] = Lbox_inv[2] = 0.00;
	
	//cout << "Box constructed" << endl;
	
}

Box::~Box()
{
	
	//cout << "Box destroyed" << endl;
	
}

// set all the dimensions of the simulation box
void Box::setDimensions(float * xi, float * yi, float * zi)
{
	
	xbox[0] = xi[0]; xbox[1] = xi[1]; 
	ybox[0] = yi[0]; ybox[1] = yi[1]; 
	zbox[0] = zi[0]; zbox[1] = zi[1]; 
	
	// calculate the length of the box in all three directions
	// also calculate the inverse of the length, which I need
	// for invoking periodic boundary conditions later
	Lbox[0] = xbox[1] - xbox[0]; Lbox_inv[0] = 1.00 / Lbox[0];
	Lbox[1] = ybox[1] - ybox[0]; Lbox_inv[1] = 1.00 / Lbox[1];
	Lbox[2] = zbox[1] - zbox[0]; Lbox_inv[2] = 1.00 / Lbox[2];

}

void Box::getDimensions(float * xlo, float * xhi, float * ylo, float * yhi, float * zlo, float * zhi) const
{
	
	*xlo = xbox[0];
	*xhi = xbox[1];
	*ylo = ybox[0];
	*yhi = ybox[1];
	*zlo = zbox[0];
	*zhi = zbox[1];
	
}

void Box::printDimensions() const
{
	
	cout << "xlow = " << xbox[0] << " xhi = " << xbox[1] << endl;
	cout << "ylow = " << ybox[0] << " yhi = " << ybox[1] << endl;
	cout << "zlow = " << zbox[0] << " zhi = " << zbox[1] << endl;

}

// return pointer to the box dimensions
float* Box::getLength( int dimension ) 
{
	
	if (dimension == 1) return Lbox;
	if (dimension == 2) return Lbox_inv;
	
	if (dimension < 1 && dimension > 2) {
		cout << "dimension must be between 1 and 2!" << endl;
		exit(1);
		return NULL;
	}
	
}

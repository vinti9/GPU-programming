// member function definitions for object Atoms

#include <iostream>
using std::cout;
using std::endl;

#include "Atoms.hpp"

Atoms::Atoms()
{
	
	//cout << "Atoms constructed" << endl;

}

Atoms::~Atoms()
{
	
	// free memory on CPU
	delete [] xx;
	delete [] yy;
	delete [] zz;

	//cout << "Atoms destroyed" << endl;
	
}

void Atoms::setNatoms( int ntmp )
{
	
	natoms = ntmp;
	
}

int Atoms::printNatoms() const
{
	
	return natoms;
	
}

void Atoms::allocateCoords()
{
	
	// allocate space on the CPU
	xx = new float[natoms];
	yy = new float[natoms];
	zz = new float[natoms];
	
}

// returns pointer to the coordinate arrays
float * Atoms::getCoords( int dimension )
{
	
	if (dimension == 1) return xx;
	if (dimension == 2) return yy;
	if (dimension == 3) return zz;
		
	if (dimension < 1 || dimension > 3) {
		cout << "dimension must be between 1 and 3!" << endl;
		exit(1);
		return NULL;
	}
		
}

void Atoms::assignCoords(int i, float xi, float yi, float zi)
{

	xx[i] = xi;
	yy[i] = yi;
	zz[i] = zi;

}

// put atomic positions in correct units
void Atoms::scaleCoords( const Box box_i ) 
{
	
	float xlo,xhi,ylo,yhi,zlo,zhi;
	box_i.getDimensions(&xlo,&xhi,&ylo,&yhi,&zlo,&zhi);
	
	for (int i=0; i<natoms ; i++) {
	
		xx[i] *= (xhi - xlo);
		xx[i] += xlo;
		yy[i] *= (yhi - ylo);
		yy[i] += ylo;
		zz[i] *= (zhi - zlo);
		zz[i] += zlo;
		
	}
	
}

void Atoms::updateCoords(float * xi, float * yi, float * zi)
{
	
	for (int i=0; i<natoms ; i++) {
	
		xx[i] += xi[i];
		yy[i] += yi[i];
		zz[i] += zi[i];
		
	}
	
}

void Atoms::printCoords() const
{
	
	cout << " xx= " << xx << 
	        " yy= " << yy <<
		    " zz= " << zz << endl;

}

void Atoms::printRasmol(ofstream & outfile) const
{
	
	outfile << natoms << endl << "Atoms" << endl; 
	for (int j = 0; j < natoms; j++)
		outfile << "Au " << xx[j] << "  " << yy[j] << "  " << zz[j] << endl;

}

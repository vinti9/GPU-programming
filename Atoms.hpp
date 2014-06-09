// class definition for class Atoms

#ifndef ATOMS_HPP
#define ATOMS_HPP

#include <fstream>
using std::ofstream;

#include "Box.hpp"

class Atoms {
	
public:
	Atoms(); // default constructor
	~Atoms(); //destructor
	void setNatoms( int );
	int printNatoms() const;
	void allocateCoords();
	float * getCoords( int );
	void assignCoords(int, float, float, float);
	void scaleCoords( const Box );
	void updateCoords(float *, float *, float *);
	void printCoords() const;
	void printRasmol( ofstream & ) const;
	
private:
	float * xx; // array of x particle positions
	float * yy; // array of y particle positions
	float * zz; // array of z particle positions
	int natoms; //number of atoms

};

#endif

// member function definitions for Position_Update class

#include <iostream>
using std::cout;
using std::endl;

#include "Position_Update.hpp"

Position_Update::Position_Update()
{
	
	ncnt = 0;
	
	//cout << "Position_Update object created" << endl;

}

Position_Update::~Position_Update()
{
	
	//cout << "Position_Update object destroyed" << endl;

}

// update the positions of the atoms randomly
// In a real MD simulation this would be done by integrating
// Newton's equations of motion
void Position_Update::randomUpdate( Atoms * dummy_atoms ) const
{
	
	int n = dummy_atoms->printNatoms();
	float xp[n],yp[n],zp[n];
	
	for (int i = 0; i < dummy_atoms->printNatoms(); i++)
	{
		
		xp[i] = rfloat(0.05);
		yp[i] = rfloat(0.05);
		zp[i] = rfloat(0.05);
		
	}
	
	dummy_atoms->updateCoords(xp,yp,zp); // change atom coordinates randomly

}

void Position_Update::incrementCnt()
{
	
	ncnt++;
	
}

int Position_Update::printCnt() const
{
	
	return ncnt;
	
}

string Position_Update::getName() const
{
	
	return "Position";
	
}

void Position_Update::print_message() const
{
	
	cout << "Updating the particle positions randomly..." << endl;
	
}

// generates a random float, this is a non-member function
float rfloat(float maxf)
{
	
	float Lo = -1.0*maxf/2.0, Hi = maxf/2.0;
	float r = Lo + (float)rand()/((float)RAND_MAX/(Hi-Lo));
	return r;

}

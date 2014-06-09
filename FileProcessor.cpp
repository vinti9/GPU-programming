// Member function definitions for class FileProcessor

#include <iostream>
using std::cout;
using std::endl;
using std::ios;

#include <iostream>
using std::ifstream;

#include "FileProcessor.hpp"
#include "Atoms.hpp"

// Constructor
FileProcessor::FileProcessor()
{
	
	//cout << "FileProcessor created" << endl;
	
}

FileProcessor::~FileProcessor()
{
	
	//cout << "FileProcessor destroyed" << endl;
	
}

// This member function reads in the header to a lammps trajectory snapshot
// The box and atoms objects can be initialized after reading in the header 
// because we determine the box size and number of atoms
void FileProcessor::getHeader(ifstream & Input_File,Box * box_i, Atoms * dummy_atoms) const
{
	
	char tmp[100];
	int t,n;
	float xi[2],yi[2],zi[2];
	
	Input_File.getline(tmp,100); // burn line
	Input_File >> t; Input_File.getline(tmp,100);
	Input_File.getline(tmp,100);
	Input_File >> n; Input_File.getline(tmp,100);
	Input_File.getline(tmp,100);
	Input_File >> xi[0] >> xi[1]; Input_File.getline(tmp,100);
	Input_File >> yi[0] >> yi[1]; Input_File.getline(tmp,100);
	Input_File >> zi[0] >> zi[1]; Input_File.getline(tmp,100);
	Input_File.getline(tmp,100);
	
	cout << "Setting dimensions of box..." << endl;
	//cout << "n= " << n << endl;
	
	box_i->setDimensions(xi,yi,zi);
	dummy_atoms->setNatoms(n);

}

// reads in the atomic coordinates, and assigns them to the atoms object
void FileProcessor::getData( ifstream & Input_File, Atoms * dummy_atoms ) const
{
	
	char tmp[500];
	int id,atype,cnt=0;
	float xtmp,ytmp,ztmp;
	
	while (Input_File >> id >> atype >> xtmp >> ytmp >> ztmp)
	{

		dummy_atoms->assignCoords(cnt,xtmp,ytmp,ztmp);
		Input_File.getline(tmp,500);
		cnt++;
		
	}

}

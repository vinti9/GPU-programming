// Class definition for cell

#ifndef CELL_HPP
#define CELL_HPP

#include "Atoms.hpp"
#include "Device_Atoms.hpp"
#include "Box.hpp"
#include "Device_Box.hpp"
#include "FileProcessor.hpp"

class Cell {
	
public:
	Cell(char * = NULL, int = 0, int = 0); // constructor
	~Cell(); // destructor
	void runSimulation(); // runs the simulation
	void setNtimesteps( int ); // sets the number of timesteps
	int printNtimesteps() const; // outputs the number of timesteps 
	void setPrintfreq( int ); // sets the print frequency
	int printPrintfreq() const; // prints the print frequency
	
private:
	Atoms atoms; // vector of atom objects
	Device_Atoms d_atoms; // device copy of atoms
	Box h_box; // CPU object containing information about the simulation box dimensions
	Device_Box d_box; // object containing box info for GPU access
	FileProcessor file_processor; // object that processes the data file of atomic coordinates
	int timesteps; // number of timesteps in simulation
	int print_freq; // print frequency
	
}; // end class cell

#endif

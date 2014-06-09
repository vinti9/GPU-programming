// member function definitions for class Cell

#include <iostream>
using std::cout;
using std::endl;
using std::ios;

#include "Cell.hpp"
#include "Position_Update.hpp"
#include "TBSMA_Compute.hpp"
#include "device_space.cuh"

Cell::Cell(char * file_i, int ts, int out_freq )
    : atoms(), 
      d_atoms(),
      h_box(),
      d_box(),
	  file_processor()
{
	
	ifstream Input_File( file_i, ios::in );
	
	// check to make sure the file can be opened
	if (!Input_File)
	{
		
		cout << "Error: file could not be opened\n" << endl;
		exit(1);
		
	}
	
	setNtimesteps( ts ); // set the number of simulation timesteps
	setPrintfreq( out_freq ); // set the frequency at which to print energy/force
	
	// process the header of the data file, grab the box coordinates while you're at it
	file_processor.getHeader( Input_File, &h_box, &atoms );
	h_box.printDimensions(); // print out the box coordinates
	d_box.setupDevicePtrs(); // setup pointers in device memory
	atoms.allocateCoords(); // allocate space for atoms on the CPU
	
	// now read in the atomic coordinates
	file_processor.getData( Input_File, &atoms ); // atoms is an object containing all the coordinates
	// atomic coordinates are currently scaled according to box dimensions
	atoms.scaleCoords( h_box ); // put the coordinates in angstroms here
	d_atoms.setupDevicePtrs(); // setup pointers for device memory
	
	// clear space for atoms, rho, and box on the device
	// we only do this once, then copy the positions of the particles from
	// the CPU every timestep. rho is never copied, it is just re-zeroed each
	// time the kernel is called. The box dimensions are just copied from the host once as they do not change.
	make_device_space(&d_atoms,&d_box,h_box,atoms.printNatoms());

	cout << "Cell constructed" << endl;

}

Cell::~Cell()
{
	
	clear_device_space( d_atoms.getDevPtr(1), d_atoms.getDevPtr(2), d_atoms.getDevPtr(3),
					   d_atoms.getDevPtr(4), d_box.getDevPtr(1), d_box.getDevPtr(2) );
	
	cout << "Cell destroyed" << endl;

}

void Cell::setNtimesteps( int ts )
{
	
	timesteps = ts;
	
}

int Cell::printNtimesteps() const
{
	
	return timesteps;
	
}

void Cell::setPrintfreq( int out_freq )
{
	
	print_freq = out_freq;
	
}

int Cell::printPrintfreq() const
{
	
	return print_freq;
	
}

void Cell::runSimulation()
{
	
	cout << "Running the simulation..." << endl;
	srand((unsigned)time(NULL)); //initialize the random seed for the RNG
	
	// The simulation is actually performed here. I calculate the 
	// interatomic energy/force on the GPU and update the positions
	// of the particles on the CPU. A high-performance code would
	// also update the particle positions on the GPU, copying the particle 
	// positions between the GPU and CPU every timestep is very inefficient.
	// This code is just a simple, proof-of-concept algorithm. I am also
	// not employing a neighborlist in this stripped down code, which is typically
	// used for improving the efficiency of the simulation by only calculating the
	// energy/force between atoms that are close enough to interact.
	//
	//
	// step 1: calculate the force/energy on the GPU
	//
	// step 2: update the positions of the particles
	// 
	// in a real simulation this would be done by integrating Newton's 
	// equations of motion, but for simplicity I update the positions
	// randomly here
	//
	TBSMA_Compute tbsmaComputer(atoms.printNatoms()); //instantiate compute object
	Compute *Compute_ptr = 0; // Compute is an abstract base class
	Compute_ptr = &tbsmaComputer; 
	
	Position_Update posUpdater; //instantiate update object
	Update *Update_ptr = 0; // Update is an abstract base class
	Update_ptr = &posUpdater;
		
	ofstream Output_File( "traj.xyz", ios::out );  // output file for atomic trajectory
	
	bool print_now = 0;
	
	for (int cnt = 0; cnt <= timesteps ; cnt++)
	{
		
		print_now = ( cnt%print_freq == 0 ) ? 1 : 0;
		
		// Calculate the energy/force acting on all particles
		Compute_ptr->compute( &atoms , &d_atoms, &d_box, print_now );
		tbsmaComputer.incrementCnt();
		if (tbsmaComputer.printCnt() == 1) {
			cout << "Compute type: " << Compute_ptr->getName() << endl;
			Compute_ptr->print_message();
		}
	
		// Update particle positions
		posUpdater.randomUpdate( &atoms );
		posUpdater.incrementCnt();
		if (posUpdater.printCnt() == 1) {
			cout << "Update type: " << Update_ptr->getName() << endl;
			Update_ptr->print_message();
		}
		
		// print out energy and coordinates (to file) every print_freq timesteps
		if (print_now) {
			
			cout << "timestep: " << cnt << " energy: " << tbsmaComputer.printEnergy() <<
			" force: " << tbsmaComputer.printForce() << endl;
		
			// Output resulting trajectory
			atoms.printRasmol( Output_File );
			
		}
		
	}
	
	Output_File.close(); // close trajectory file

}

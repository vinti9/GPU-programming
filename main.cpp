//************************************************
//
//
// This is a stripped down version of the tb-sma 
// CUDA code that calculates the tb-sma energy/force
// on a GPU.
//
//
// Authored by: William French
// Last Revision: 03/19/2013
//
//************************************************

#include <iostream>
using std::endl;
using std::cout;

#include "Cell.hpp"

int main(int argc, char* argv [])
{

    // check input arguments
    if (argc != 4)
    {
		
        cout << "Arguments: trajectory-file-name timesteps output-frequency" << endl;
        exit(1);

    }

    // process input arguments
    char file_name[1000];
    sprintf( file_name, "%s", argv[1] ); // data file containing atomic coordinates
    int timesteps = atoi( argv[2] ); // number of timesteps to be performed in the simulation
	int output_freq = atoi( argv[3] ); // the frequency at which energy/force are output to screen
	
	// instantiate the whole simulation cell containing the box, atoms, etc.
	Cell cell( file_name, timesteps, output_freq );
	
	// run the simulation
	cell.runSimulation();
	
	cout << "Simulation Complete" << endl;

    return 0;

}

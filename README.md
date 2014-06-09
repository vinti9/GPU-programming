GPU-programming
===============

A small project written in C++ and CUDA for computing the energy/force between a group of atoms and updating the positions of atoms using Newton's equations of motion.
To compile from the command line, just type:

./compile

This assumes your cuda environment is setup and nvcc 
can be invoked from anywhere on your machine. You may
also need to make "compile" executable (chmod +x compile).

To run type:

./tbsma-gpu coordinates.lammpstrj 10000 100

where coordinates.lammpstrj is a data file containing 
the coordinates of a small gold nanowire, 10000 is the 
number of timesteps you want to run the simulation, and 100
is the frequency at which you want to output the total
energy/force of the simulation.

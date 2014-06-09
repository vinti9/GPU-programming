// definition for abstract Compute class

#ifndef COMPUTE_HPP
#define COMPUTE_HPP

#include <string>
using std::string;

#include "Atoms.hpp"
#include "Device_Atoms.hpp"
#include "Device_Box.hpp"

class Compute{
	
public:
	
	virtual void compute( Atoms * , Device_Atoms * , Device_Box * , bool ) = 0;
	virtual string getName() const = 0;
	virtual void print_message() const = 0;
	
};

#endif

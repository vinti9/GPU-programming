// Class definition for Position_Update
// This is a derived class from the Update class

#ifndef POSITION_UPDATE_HPP
#define POSITION_UPDATE_HPP

#include <string>
using std::string;

#include "Update.hpp"
#include "Atoms.hpp"

class Position_Update : public Update {
	
	
public:
	Position_Update();  // constructor
	~Position_Update(); // destructor
	void randomUpdate( Atoms * ) const;
	void incrementCnt();
	int printCnt() const;
	virtual string getName() const; // prints the name of the Update
	virtual void print_message() const; // prints a message to describe the function of this Update
	
	
private:
	int ncnt; //counter
	
	
};

float rfloat( float ); // nonmember function that generates a random float
#endif

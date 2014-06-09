// definition for class FileProcessor

#ifndef FILEPROCESSOR_HPP
#define FILEPROCESSOR_HPP

#include <fstream>
using std::ifstream;

#include "Atoms.hpp"
#include "Box.hpp"


class FileProcessor {
	
public:
	FileProcessor(); //constructor
	~FileProcessor(); //destructor
	void getHeader( ifstream & , Box *, Atoms * ) const;  //read in the header of the file
	void getData( ifstream & , Atoms * ) const; // read in the atomic coordinates
	
private:

};

#endif 

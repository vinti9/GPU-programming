// Class definition for abstract class Update

#ifndef UPDATE_HPP
#define UPDATE_HPP

#include <string>
using std::string;

class Update{
	
public:

	virtual string getName() const = 0;
	virtual void print_message() const = 0;

};

#endif

// class definition for class Box

#ifndef BOX_HPP
#define BOX_HPP


class Box {
	
public:
	Box(); //constructor
	~Box(); //destructor
	void printDimensions() const;
	void setDimensions( float * , float * , float * );
	void getDimensions( float * , float * , float * , float * , float * , float * ) const;
	float* getLength( int ); // return pointer to box dimensions
	
private:
	float xbox[2]; // box boundaries in x direction
	float ybox[2];
	float zbox[2];
	float Lbox[3]; // length of box in x,y,and z
	float Lbox_inv[3]; // inverse of the box dimensions in x, y, and z

};

#endif

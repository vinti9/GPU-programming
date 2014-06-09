// class definition for class Device_Box

#ifndef DEVICE_BOX_HPP
#define DEVICE_BOX_HPP


class Device_Box {
	
public:
	Device_Box(); //constructor
	~Device_Box(); //destructor
	float ** getDevPtr( int );
	void setupDevicePtrs();
	
private:
	float * Lbox;
	float * Lbox_inv;
	float ** Lbox_ptr;
	float ** Lbox_inv_ptr;

};

#endif

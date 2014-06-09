// function prototype for creating space on device for atoms

#ifndef __DEVICE_SPACE_CUH__
#define __DEVICE_SPACE_CUH__

#include "Device_Atoms.hpp"
#include "Device_Box.hpp"
#include "Box.hpp"

void make_device_space( Device_Atoms *, Device_Box *, Box, int );

void clear_device_space( float **, float **, float **, float ** , float **, float ** );

void make_space_compute( float **, float **, int );

void clear_space_compute( float **, float ** );

#endif

// function definition for cuda calling function

#ifndef __TBSMAForceKernel_CUH__
#define __TBSMAForceKernel_CUH__

#include "Atoms.hpp"
#include "Device_Atoms.hpp"
#include "Device_Box.hpp"

void gpu_compute_TBSMA( Atoms * , Device_Atoms *, Device_Box *, float *, float *, float **, float **, bool );

#endif

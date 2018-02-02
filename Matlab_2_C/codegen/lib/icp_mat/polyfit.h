//
// File: polyfit.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//
#ifndef POLYFIT_H
#define POLYFIT_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "icp_mat_types.h"

// Function Declarations
extern void b_polyfit(const double x[3], const double y[3], double p[3]);
extern void polyfit(const double x[3], const double y[3], double p[2]);

#endif

//
// File trailer for polyfit.h
//
// [EOF]
//

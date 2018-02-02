//
// File: xnrm2.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//
#ifndef XNRM2_H
#define XNRM2_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "icp_mat_types.h"

// Function Declarations
extern double b_xnrm2(int n, const double x[3], int ix0);
extern double c_xnrm2(const double x[6], int ix0);
extern double d_xnrm2(int n, const double x[6], int ix0);
extern double e_xnrm2(const double x[9], int ix0);
extern double xnrm2(int n, const double x[9], int ix0);

#endif

//
// File trailer for xnrm2.h
//
// [EOF]
//

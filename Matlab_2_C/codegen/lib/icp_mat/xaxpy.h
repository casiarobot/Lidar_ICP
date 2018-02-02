//
// File: xaxpy.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//
#ifndef XAXPY_H
#define XAXPY_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "icp_mat_types.h"

// Function Declarations
extern void b_xaxpy(int n, double a, const double x[9], int ix0, double y[3],
                    int iy0);
extern void c_xaxpy(int n, double a, const double x[3], int ix0, double y[9],
                    int iy0);
extern void xaxpy(int n, double a, int ix0, double y[9], int iy0);

#endif

//
// File trailer for xaxpy.h
//
// [EOF]
//

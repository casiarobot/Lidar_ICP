//
// File: icp_mat.h
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//
#ifndef ICP_MAT_H
#define ICP_MAT_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "icp_mat_types.h"

// Function Declarations
extern void icp_mat(const double q[1077], const double p[1077], double Tt[3],
                    double Tr[9], double ER[16], double *t);

#endif

//
// File trailer for icp_mat.h
//
// [EOF]
//

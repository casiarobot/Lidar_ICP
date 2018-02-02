//
// File: xscal.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "xscal.h"

// Function Definitions

//
// Arguments    : double a
//                double x[9]
//                int ix0
// Return Type  : void
//
void xscal(double a, double x[9], int ix0)
{
  int k;
  for (k = ix0; k <= ix0 + 2; k++) {
    x[k - 1] *= a;
  }
}

//
// File trailer for xscal.cpp
//
// [EOF]
//

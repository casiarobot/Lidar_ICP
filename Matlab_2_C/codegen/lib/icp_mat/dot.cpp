//
// File: dot.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "dot.h"

// Function Definitions

//
// Arguments    : const double a[7]
//                const double b[7]
// Return Type  : double
//
double dot(const double a[7], const double b[7])
{
  double c;
  int k;
  c = 0.0;
  for (k = 0; k < 7; k++) {
    c += a[k] * b[k];
  }

  return c;
}

//
// File trailer for dot.cpp
//
// [EOF]
//

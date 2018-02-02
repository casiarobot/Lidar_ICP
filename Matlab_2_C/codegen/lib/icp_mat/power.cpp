//
// File: power.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "power.h"

// Function Definitions

//
// Arguments    : const double a[1077]
//                double y[1077]
// Return Type  : void
//
void b_power(const double a[1077], double y[1077])
{
  int k;
  for (k = 0; k < 1077; k++) {
    y[k] = a[k] * a[k];
  }
}

//
// Arguments    : const double a[359]
//                double y[359]
// Return Type  : void
//
void power(const double a[359], double y[359])
{
  int k;
  for (k = 0; k < 359; k++) {
    y[k] = a[k] * a[k];
  }
}

//
// File trailer for power.cpp
//
// [EOF]
//

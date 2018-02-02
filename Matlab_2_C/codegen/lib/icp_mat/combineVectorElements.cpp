//
// File: combineVectorElements.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "combineVectorElements.h"

// Function Definitions

//
// Arguments    : const double x[359]
// Return Type  : double
//
double combineVectorElements(const double x[359])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 358; k++) {
    y += x[k + 1];
  }

  return y;
}

//
// File trailer for combineVectorElements.cpp
//
// [EOF]
//

//
// File: ixamax.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "ixamax.h"

// Function Definitions

//
// Arguments    : int n
//                const double x[3]
//                int ix0
// Return Type  : int
//
int b_ixamax(int n, const double x[3], int ix0)
{
  int idxmax;
  int ix;
  double smax;
  int k;
  double s;
  if (n < 1) {
    idxmax = 0;
  } else {
    idxmax = 1;
    if (n > 1) {
      ix = ix0 - 1;
      smax = std::abs(x[ix0 - 1]);
      for (k = 2; k <= n; k++) {
        ix++;
        s = std::abs(x[ix]);
        if (s > smax) {
          idxmax = k;
          smax = s;
        }
      }
    }
  }

  return idxmax;
}

//
// Arguments    : int n
//                const double x[2]
//                int ix0
// Return Type  : int
//
int ixamax(int n, const double x[2], int ix0)
{
  int idxmax;
  int ix;
  double smax;
  int k;
  double s;
  if (n < 1) {
    idxmax = 0;
  } else {
    idxmax = 1;
    if (n > 1) {
      ix = ix0 - 1;
      smax = std::abs(x[ix0 - 1]);
      for (k = 2; k <= n; k++) {
        ix++;
        s = std::abs(x[ix]);
        if (s > smax) {
          idxmax = k;
          smax = s;
        }
      }
    }
  }

  return idxmax;
}

//
// File trailer for ixamax.cpp
//
// [EOF]
//

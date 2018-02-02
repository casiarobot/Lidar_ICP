//
// File: svd.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "svd.h"
#include "svd1.h"

// Function Definitions

//
// Arguments    : const double A[9]
//                double U[9]
//                double S[9]
//                double V[9]
// Return Type  : void
//
void svd(const double A[9], double U[9], double S[9], double V[9])
{
  boolean_T p;
  int i;
  double dv7[9];
  double s[3];
  double U1[9];
  double V1[9];
  p = true;
  for (i = 0; i < 9; i++) {
    if (p && ((!rtIsInf(A[i])) && (!rtIsNaN(A[i])))) {
      p = true;
    } else {
      p = false;
    }
  }

  if (p) {
    b_svd(A, U, s, V);
  } else {
    memset(&dv7[0], 0, 9U * sizeof(double));
    b_svd(dv7, U1, s, V1);
    for (i = 0; i < 9; i++) {
      U[i] = rtNaN;
    }

    for (i = 0; i < 3; i++) {
      s[i] = rtNaN;
    }

    for (i = 0; i < 9; i++) {
      V[i] = rtNaN;
    }
  }

  memset(&S[0], 0, 9U * sizeof(double));
  for (i = 0; i < 3; i++) {
    S[i + 3 * i] = s[i];
  }
}

//
// File trailer for svd.cpp
//
// [EOF]
//

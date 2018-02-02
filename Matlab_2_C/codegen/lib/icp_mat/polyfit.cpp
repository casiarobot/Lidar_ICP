//
// File: polyfit.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "polyfit.h"
#include "xgeqp3.h"

// Function Definitions

//
// Arguments    : const double x[3]
//                const double y[3]
//                double p[3]
// Return Type  : void
//
void b_polyfit(const double x[3], const double y[3], double p[3])
{
  int k;
  double V[9];
  double tau[3];
  int jpvt[3];
  int i;
  double B[3];
  double wj;
  for (k = 0; k < 3; k++) {
    V[6 + k] = 1.0;
    V[3 + k] = x[k];
    V[k] = x[k] * V[3 + k];
    B[k] = y[k];
  }

  b_xgeqp3(V, tau, jpvt);
  for (k = 0; k < 3; k++) {
    p[k] = 0.0;
    if (tau[k] != 0.0) {
      wj = B[k];
      for (i = k + 1; i + 1 < 4; i++) {
        wj += V[i + 3 * k] * B[i];
      }

      wj *= tau[k];
      if (wj != 0.0) {
        B[k] -= wj;
        for (i = k + 1; i + 1 < 4; i++) {
          B[i] -= V[i + 3 * k] * wj;
        }
      }
    }
  }

  for (i = 0; i < 3; i++) {
    p[jpvt[i] - 1] = B[i];
  }

  for (k = 2; k >= 0; k--) {
    p[jpvt[k] - 1] /= V[k + 3 * k];
    for (i = 0; i + 1 <= k; i++) {
      p[jpvt[i] - 1] -= p[jpvt[k] - 1] * V[i + 3 * k];
    }
  }
}

//
// Arguments    : const double x[3]
//                const double y[3]
//                double p[2]
// Return Type  : void
//
void polyfit(const double x[3], const double y[3], double p[2])
{
  int k;
  double V[6];
  double tau[2];
  int jpvt[2];
  double B[3];
  int i;
  double wj;
  for (k = 0; k < 3; k++) {
    V[3 + k] = 1.0;
    V[k] = x[k];
    B[k] = y[k];
  }

  xgeqp3(V, tau, jpvt);
  for (k = 0; k < 2; k++) {
    p[k] = 0.0;
    if (tau[k] != 0.0) {
      wj = B[k];
      for (i = k + 1; i + 1 < 4; i++) {
        wj += V[i + 3 * k] * B[i];
      }

      wj *= tau[k];
      if (wj != 0.0) {
        B[k] -= wj;
        for (i = k + 1; i + 1 < 4; i++) {
          B[i] -= V[i + 3 * k] * wj;
        }
      }
    }
  }

  for (i = 0; i < 2; i++) {
    p[jpvt[i] - 1] = B[i];
  }

  for (k = 1; k >= 0; k--) {
    p[jpvt[k] - 1] /= V[k + 3 * k];
    i = 1;
    while (i <= k) {
      p[jpvt[0] - 1] -= p[jpvt[1] - 1] * V[3];
      i = 2;
    }
  }
}

//
// File trailer for polyfit.cpp
//
// [EOF]
//

//
// File: xrotg.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "xrotg.h"

// Function Definitions

//
// Arguments    : double *a
//                double *b
//                double *c
//                double *s
// Return Type  : void
//
void xrotg(double *a, double *b, double *c, double *s)
{
  double roe;
  double absa;
  double absb;
  double scale;
  double ads;
  double bds;
  roe = *b;
  absa = std::abs(*a);
  absb = std::abs(*b);
  if (absa > absb) {
    roe = *a;
  }

  scale = absa + absb;
  if (scale == 0.0) {
    *s = 0.0;
    *c = 1.0;
    scale = 0.0;
    *b = 0.0;
  } else {
    ads = absa / scale;
    bds = absb / scale;
    scale *= std::sqrt(ads * ads + bds * bds);
    if (roe < 0.0) {
      scale = -scale;
    }

    *c = *a / scale;
    *s = *b / scale;
    if (absa > absb) {
      *b = *s;
    } else if (*c != 0.0) {
      *b = 1.0 / *c;
    } else {
      *b = 1.0;
    }
  }

  *a = scale;
}

//
// File trailer for xrotg.cpp
//
// [EOF]
//

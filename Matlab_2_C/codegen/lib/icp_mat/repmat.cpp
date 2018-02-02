//
// File: repmat.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "repmat.h"

// Function Definitions

//
// Arguments    : const double a[3]
//                double b[1077]
// Return Type  : void
//
void b_repmat(const double a[3], double b[1077])
{
  int jtilecol;
  int ibtile;
  int k;
  for (jtilecol = 0; jtilecol < 359; jtilecol++) {
    ibtile = jtilecol * 3;
    for (k = 0; k < 3; k++) {
      b[ibtile + k] = a[k];
    }
  }
}

//
// Arguments    : const double a[359]
//                double b[1077]
// Return Type  : void
//
void c_repmat(const double a[359], double b[1077])
{
  int jcol;
  int ibmat;
  int itilerow;
  for (jcol = 0; jcol < 359; jcol++) {
    ibmat = jcol * 3;
    for (itilerow = 0; itilerow < 3; itilerow++) {
      b[ibmat + itilerow] = a[jcol];
    }
  }
}

//
// Arguments    : double b[144]
// Return Type  : void
//
void repmat(double b[144])
{
  int jtilecol;
  int ibtile;
  int jcol;
  int iacol;
  int ibmat;
  int k;
  static const signed char iv1[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  for (jtilecol = 0; jtilecol < 16; jtilecol++) {
    ibtile = jtilecol * 9;
    for (jcol = 0; jcol < 3; jcol++) {
      iacol = jcol * 3;
      ibmat = ibtile + jcol * 3;
      for (k = 0; k < 3; k++) {
        b[ibmat + k] = iv1[iacol + k];
      }
    }
  }
}

//
// File trailer for repmat.cpp
//
// [EOF]
//

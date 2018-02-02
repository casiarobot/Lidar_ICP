//
// File: xgeqp3.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "xgeqp3.h"
#include "xnrm2.h"
#include "ixamax.h"

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

//
// Arguments    : double A[9]
//                double tau[3]
//                int jpvt[3]
// Return Type  : void
//
void b_xgeqp3(double A[9], double tau[3], int jpvt[3])
{
  int k;
  int iy;
  int i;
  double work[3];
  int i_i;
  double temp;
  int pvt;
  double vn1[3];
  double vn2[3];
  int ix;
  double atmp;
  double temp2;
  int itemp;
  int i_ip1;
  int i8;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  k = 1;
  for (iy = 0; iy < 3; iy++) {
    jpvt[iy] = 1 + iy;
    work[iy] = 0.0;
    temp = e_xnrm2(A, k);
    vn2[iy] = temp;
    k += 3;
    vn1[iy] = temp;
  }

  for (i = 0; i < 3; i++) {
    i_i = i + i * 3;
    pvt = (i + b_ixamax(3 - i, vn1, i + 1)) - 1;
    if (pvt + 1 != i + 1) {
      ix = 3 * pvt;
      iy = 3 * i;
      for (k = 0; k < 3; k++) {
        temp = A[ix];
        A[ix] = A[iy];
        A[iy] = temp;
        ix++;
        iy++;
      }

      itemp = jpvt[pvt];
      jpvt[pvt] = jpvt[i];
      jpvt[i] = itemp;
      vn1[pvt] = vn1[i];
      vn2[pvt] = vn2[i];
    }

    if (i + 1 < 3) {
      atmp = A[i_i];
      temp2 = 0.0;
      temp = xnrm2(2 - i, A, i_i + 2);
      if (temp != 0.0) {
        temp = rt_hypotd_snf(A[i_i], temp);
        if (A[i_i] >= 0.0) {
          temp = -temp;
        }

        if (std::abs(temp) < 1.0020841800044864E-292) {
          itemp = 0;
          i8 = (i_i - i) + 3;
          do {
            itemp++;
            for (k = i_i + 1; k + 1 <= i8; k++) {
              A[k] *= 9.9792015476736E+291;
            }

            temp *= 9.9792015476736E+291;
            atmp *= 9.9792015476736E+291;
          } while (!(std::abs(temp) >= 1.0020841800044864E-292));

          temp = rt_hypotd_snf(atmp, xnrm2(2 - i, A, i_i + 2));
          if (atmp >= 0.0) {
            temp = -temp;
          }

          temp2 = (temp - atmp) / temp;
          atmp = 1.0 / (atmp - temp);
          i8 = (i_i - i) + 3;
          for (k = i_i + 1; k + 1 <= i8; k++) {
            A[k] *= atmp;
          }

          for (k = 1; k <= itemp; k++) {
            temp *= 1.0020841800044864E-292;
          }

          atmp = temp;
        } else {
          temp2 = (temp - A[i_i]) / temp;
          atmp = 1.0 / (A[i_i] - temp);
          i8 = (i_i - i) + 3;
          for (k = i_i + 1; k + 1 <= i8; k++) {
            A[k] *= atmp;
          }

          atmp = temp;
        }
      }

      tau[i] = temp2;
      A[i_i] = atmp;
      atmp = A[i_i];
      A[i_i] = 1.0;
      i_ip1 = (i + (i + 1) * 3) + 1;
      if (tau[i] != 0.0) {
        lastv = 3 - i;
        itemp = (i_i - i) + 2;
        while ((lastv > 0) && (A[itemp] == 0.0)) {
          lastv--;
          itemp--;
        }

        lastc = 2 - i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          itemp = i_ip1 + (lastc - 1) * 3;
          k = itemp;
          do {
            exitg1 = 0;
            if (k <= (itemp + lastv) - 1) {
              if (A[k - 1] != 0.0) {
                exitg1 = 1;
              } else {
                k++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          for (iy = 1; iy <= lastc; iy++) {
            work[iy - 1] = 0.0;
          }

          iy = 0;
          i8 = i_ip1 + 3 * (lastc - 1);
          for (itemp = i_ip1; itemp <= i8; itemp += 3) {
            ix = i_i;
            temp = 0.0;
            pvt = (itemp + lastv) - 1;
            for (k = itemp; k <= pvt; k++) {
              temp += A[k - 1] * A[ix];
              ix++;
            }

            work[iy] += temp;
            iy++;
          }
        }

        if (!(-tau[i] == 0.0)) {
          itemp = i_ip1 - 1;
          pvt = 0;
          for (iy = 1; iy <= lastc; iy++) {
            if (work[pvt] != 0.0) {
              temp = work[pvt] * -tau[i];
              ix = i_i;
              i8 = lastv + itemp;
              for (k = itemp; k + 1 <= i8; k++) {
                A[k] += A[ix] * temp;
                ix++;
              }
            }

            pvt++;
            itemp += 3;
          }
        }
      }

      A[i_i] = atmp;
    } else {
      tau[2] = 0.0;
    }

    for (iy = i + 1; iy + 1 < 4; iy++) {
      if (vn1[iy] != 0.0) {
        temp = std::abs(A[i + 3 * iy]) / vn1[iy];
        temp = 1.0 - temp * temp;
        if (temp < 0.0) {
          temp = 0.0;
        }

        temp2 = vn1[iy] / vn2[iy];
        temp2 = temp * (temp2 * temp2);
        if (temp2 <= 1.4901161193847656E-8) {
          if (i + 1 < 3) {
            vn1[iy] = xnrm2(2 - i, A, (i + 3 * iy) + 2);
            vn2[iy] = vn1[iy];
          } else {
            vn1[iy] = 0.0;
            vn2[iy] = 0.0;
          }
        } else {
          vn1[iy] *= std::sqrt(temp);
        }
      }
    }
  }
}

//
// Arguments    : double A[6]
//                double tau[2]
//                int jpvt[2]
// Return Type  : void
//
void xgeqp3(double A[6], double tau[2], int jpvt[2])
{
  int k;
  int iy;
  int i;
  double work[2];
  int i_i;
  double temp;
  int pvt;
  double vn1[2];
  double vn2[2];
  int ix;
  double atmp;
  double temp2;
  int i7;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  k = 1;
  for (iy = 0; iy < 2; iy++) {
    jpvt[iy] = 1 + iy;
    work[iy] = 0.0;
    temp = c_xnrm2(A, k);
    vn2[iy] = temp;
    k += 3;
    vn1[iy] = temp;
  }

  for (i = 0; i < 2; i++) {
    i_i = i + i * 3;
    pvt = (i + ixamax(2 - i, vn1, i + 1)) - 1;
    if (pvt + 1 != i + 1) {
      ix = 3 * pvt;
      iy = 3 * i;
      for (k = 0; k < 3; k++) {
        temp = A[ix];
        A[ix] = A[iy];
        A[iy] = temp;
        ix++;
        iy++;
      }

      iy = jpvt[pvt];
      jpvt[pvt] = jpvt[i];
      jpvt[i] = iy;
      vn1[pvt] = vn1[i];
      vn2[pvt] = vn2[i];
    }

    atmp = A[i_i];
    temp2 = 0.0;
    temp = d_xnrm2(2 - i, A, i_i + 2);
    if (temp != 0.0) {
      temp = rt_hypotd_snf(A[i_i], temp);
      if (A[i_i] >= 0.0) {
        temp = -temp;
      }

      if (std::abs(temp) < 1.0020841800044864E-292) {
        iy = 0;
        i7 = (i_i - i) + 3;
        do {
          iy++;
          for (k = i_i + 1; k + 1 <= i7; k++) {
            A[k] *= 9.9792015476736E+291;
          }

          temp *= 9.9792015476736E+291;
          atmp *= 9.9792015476736E+291;
        } while (!(std::abs(temp) >= 1.0020841800044864E-292));

        temp = rt_hypotd_snf(atmp, d_xnrm2(2 - i, A, i_i + 2));
        if (atmp >= 0.0) {
          temp = -temp;
        }

        temp2 = (temp - atmp) / temp;
        atmp = 1.0 / (atmp - temp);
        i7 = (i_i - i) + 3;
        for (k = i_i + 1; k + 1 <= i7; k++) {
          A[k] *= atmp;
        }

        for (k = 1; k <= iy; k++) {
          temp *= 1.0020841800044864E-292;
        }

        atmp = temp;
      } else {
        temp2 = (temp - A[i_i]) / temp;
        atmp = 1.0 / (A[i_i] - temp);
        i7 = (i_i - i) + 3;
        for (k = i_i + 1; k + 1 <= i7; k++) {
          A[k] *= atmp;
        }

        atmp = temp;
      }
    }

    tau[i] = temp2;
    A[i_i] = atmp;
    if (i + 1 < 2) {
      atmp = A[i_i];
      A[i_i] = 1.0;
      if (tau[0] != 0.0) {
        lastv = 3;
        iy = i_i + 2;
        while ((lastv > 0) && (A[iy] == 0.0)) {
          lastv--;
          iy--;
        }

        lastc = 1;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          iy = 4;
          do {
            exitg1 = 0;
            if (iy <= lastv + 3) {
              if (A[iy - 1] != 0.0) {
                exitg1 = 1;
              } else {
                iy++;
              }
            } else {
              lastc = 0;
              exitg1 = 2;
            }
          } while (exitg1 == 0);

          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }

      if (lastv > 0) {
        if (lastc != 0) {
          ix = i_i;
          temp = 0.0;
          for (iy = 4; iy <= lastv + 3; iy++) {
            temp += A[iy - 1] * A[ix];
            ix++;
          }

          work[0] = temp;
        }

        if (!(-tau[0] == 0.0)) {
          pvt = 3;
          k = 0;
          iy = 1;
          while (iy <= lastc) {
            if (work[k] != 0.0) {
              temp = work[k] * -tau[0];
              ix = i_i;
              i7 = lastv + pvt;
              for (iy = pvt; iy + 1 <= i7; iy++) {
                A[iy] += A[ix] * temp;
                ix++;
              }
            }

            k++;
            pvt += 3;
            iy = 2;
          }
        }
      }

      A[i_i] = atmp;
    }

    iy = i + 2;
    while (iy < 3) {
      if (vn1[1] != 0.0) {
        temp = std::abs(A[3 + i]) / vn1[1];
        temp = 1.0 - temp * temp;
        if (temp < 0.0) {
          temp = 0.0;
        }

        temp2 = vn1[1] / vn2[1];
        temp2 = temp * (temp2 * temp2);
        if (temp2 <= 1.4901161193847656E-8) {
          vn1[1] = d_xnrm2(2 - i, A, i + 5);
          vn2[1] = vn1[1];
        } else {
          vn1[1] *= std::sqrt(temp);
        }
      }

      iy = 3;
    }
  }
}

//
// File trailer for xgeqp3.cpp
//
// [EOF]
//

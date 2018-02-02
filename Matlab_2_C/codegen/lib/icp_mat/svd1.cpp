//
// File: svd1.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "svd1.h"
#include "xrot.h"
#include "xrotg.h"
#include "sqrt.h"
#include "xswap.h"
#include "xscal.h"
#include "xaxpy.h"
#include "xdotc.h"
#include "xnrm2.h"

// Function Definitions

//
// Arguments    : const double A[9]
//                double U[9]
//                double s[3]
//                double V[9]
// Return Type  : void
//
void b_svd(const double A[9], double U[9], double s[3], double V[9])
{
  double b_A[9];
  int i;
  double b_s[3];
  double e[3];
  int q;
  double work[3];
  int m;
  int qq;
  boolean_T apply_transform;
  double nrm;
  int k;
  double r;
  int iter;
  double snorm;
  double rt;
  int exitg1;
  boolean_T exitg2;
  double f;
  double varargin_1[5];
  double mtmp;
  double sqds;
  memcpy(&b_A[0], &A[0], 9U * sizeof(double));
  for (i = 0; i < 3; i++) {
    b_s[i] = 0.0;
    e[i] = 0.0;
    work[i] = 0.0;
  }

  memset(&U[0], 0, 9U * sizeof(double));
  memset(&V[0], 0, 9U * sizeof(double));
  for (q = 0; q < 2; q++) {
    qq = q + 3 * q;
    apply_transform = false;
    nrm = xnrm2(3 - q, b_A, qq + 1);
    if (nrm > 0.0) {
      apply_transform = true;
      if (b_A[qq] < 0.0) {
        b_s[q] = -nrm;
      } else {
        b_s[q] = nrm;
      }

      if (std::abs(b_s[q]) >= 1.0020841800044864E-292) {
        r = 1.0 / b_s[q];
        i = (qq - q) + 3;
        for (k = qq; k + 1 <= i; k++) {
          b_A[k] *= r;
        }
      } else {
        i = (qq - q) + 3;
        for (k = qq; k + 1 <= i; k++) {
          b_A[k] /= b_s[q];
        }
      }

      b_A[qq]++;
      b_s[q] = -b_s[q];
    } else {
      b_s[q] = 0.0;
    }

    for (k = q + 1; k + 1 < 4; k++) {
      i = q + 3 * k;
      if (apply_transform) {
        xaxpy(3 - q, -(xdotc(3 - q, b_A, qq + 1, b_A, i + 1) / b_A[q + 3 * q]),
              qq + 1, b_A, i + 1);
      }

      e[k] = b_A[i];
    }

    for (k = q; k + 1 < 4; k++) {
      U[k + 3 * q] = b_A[k + 3 * q];
    }

    if (q + 1 <= 1) {
      nrm = b_xnrm2(2, e, 2);
      if (nrm == 0.0) {
        e[0] = 0.0;
      } else {
        if (e[1] < 0.0) {
          r = -nrm;
        } else {
          r = nrm;
        }

        if (e[1] < 0.0) {
          e[0] = -nrm;
        } else {
          e[0] = nrm;
        }

        if (std::abs(r) >= 1.0020841800044864E-292) {
          r = 1.0 / r;
          for (k = 1; k + 1 < 4; k++) {
            e[k] *= r;
          }
        } else {
          for (k = 1; k + 1 < 4; k++) {
            e[k] /= r;
          }
        }

        e[1]++;
        e[0] = -e[0];
        for (k = 2; k < 4; k++) {
          work[k - 1] = 0.0;
        }

        for (k = 1; k + 1 < 4; k++) {
          b_xaxpy(2, e[k], b_A, 3 * k + 2, work, 2);
        }

        for (k = 1; k + 1 < 4; k++) {
          c_xaxpy(2, -e[k] / e[1], work, 2, b_A, 3 * k + 2);
        }
      }

      for (k = 1; k + 1 < 4; k++) {
        V[k] = e[k];
      }
    }
  }

  m = 1;
  b_s[2] = b_A[8];
  e[1] = b_A[7];
  e[2] = 0.0;
  for (k = 0; k < 3; k++) {
    U[6 + k] = 0.0;
  }

  U[8] = 1.0;
  for (q = 1; q >= 0; q--) {
    qq = q + 3 * q;
    if (b_s[q] != 0.0) {
      for (k = q + 1; k + 1 < 4; k++) {
        i = (q + 3 * k) + 1;
        xaxpy(3 - q, -(xdotc(3 - q, U, qq + 1, U, i) / U[qq]), qq + 1, U, i);
      }

      for (k = q; k + 1 < 4; k++) {
        U[k + 3 * q] = -U[k + 3 * q];
      }

      U[qq]++;
      if (1 <= q) {
        U[3] = 0.0;
      }
    } else {
      for (k = 0; k < 3; k++) {
        U[k + 3 * q] = 0.0;
      }

      U[qq] = 1.0;
    }
  }

  for (q = 2; q >= 0; q--) {
    if ((q + 1 <= 1) && (e[0] != 0.0)) {
      for (k = 2; k < 4; k++) {
        i = 3 * (k - 1) + 2;
        xaxpy(2, -(xdotc(2, V, 2, V, i) / V[1]), 2, V, i);
      }
    }

    for (k = 0; k < 3; k++) {
      V[k + 3 * q] = 0.0;
    }

    V[q + 3 * q] = 1.0;
  }

  for (q = 0; q < 3; q++) {
    nrm = e[q];
    if (b_s[q] != 0.0) {
      rt = std::abs(b_s[q]);
      r = b_s[q] / rt;
      b_s[q] = rt;
      if (q + 1 < 3) {
        nrm = e[q] / r;
      }

      xscal(r, U, 1 + 3 * q);
    }

    if ((q + 1 < 3) && (nrm != 0.0)) {
      rt = std::abs(nrm);
      r = rt / nrm;
      nrm = rt;
      b_s[q + 1] *= r;
      xscal(r, V, 1 + 3 * (q + 1));
    }

    e[q] = nrm;
  }

  iter = 0;
  snorm = 0.0;
  for (k = 0; k < 3; k++) {
    nrm = std::abs(b_s[k]);
    r = std::abs(e[k]);
    if ((nrm > r) || rtIsNaN(r)) {
    } else {
      nrm = r;
    }

    if (!((snorm > nrm) || rtIsNaN(nrm))) {
      snorm = nrm;
    }
  }

  while ((m + 2 > 0) && (!(iter >= 75))) {
    k = m;
    do {
      exitg1 = 0;
      q = k + 1;
      if (k + 1 == 0) {
        exitg1 = 1;
      } else {
        nrm = std::abs(e[k]);
        if ((nrm <= 2.2204460492503131E-16 * (std::abs(b_s[k]) + std::abs(b_s[k
               + 1]))) || (nrm <= 1.0020841800044864E-292) || ((iter > 20) &&
             (nrm <= 2.2204460492503131E-16 * snorm))) {
          e[k] = 0.0;
          exitg1 = 1;
        } else {
          k--;
        }
      }
    } while (exitg1 == 0);

    if (k + 1 == m + 1) {
      i = 4;
    } else {
      qq = m + 2;
      i = m + 2;
      exitg2 = false;
      while ((!exitg2) && (i >= k + 1)) {
        qq = i;
        if (i == k + 1) {
          exitg2 = true;
        } else {
          nrm = 0.0;
          if (i < m + 2) {
            nrm = std::abs(e[i - 1]);
          }

          if (i > k + 2) {
            nrm += std::abs(e[i - 2]);
          }

          r = std::abs(b_s[i - 1]);
          if ((r <= 2.2204460492503131E-16 * nrm) || (r <=
               1.0020841800044864E-292)) {
            b_s[i - 1] = 0.0;
            exitg2 = true;
          } else {
            i--;
          }
        }
      }

      if (qq == k + 1) {
        i = 3;
      } else if (qq == m + 2) {
        i = 1;
      } else {
        i = 2;
        q = qq;
      }
    }

    switch (i) {
     case 1:
      f = e[m];
      e[m] = 0.0;
      for (k = m; k + 1 >= q + 1; k--) {
        xrotg(&b_s[k], &f, &nrm, &r);
        if (k + 1 > q + 1) {
          f = -r * e[0];
          e[0] *= nrm;
        }

        xrot(V, 1 + 3 * k, 1 + 3 * (m + 1), nrm, r);
      }
      break;

     case 2:
      f = e[q - 1];
      e[q - 1] = 0.0;
      for (k = q; k + 1 <= m + 2; k++) {
        xrotg(&b_s[k], &f, &nrm, &r);
        f = -r * e[k];
        e[k] *= nrm;
        xrot(U, 1 + 3 * k, 1 + 3 * (q - 1), nrm, r);
      }
      break;

     case 3:
      varargin_1[0] = std::abs(b_s[m + 1]);
      varargin_1[1] = std::abs(b_s[m]);
      varargin_1[2] = std::abs(e[m]);
      varargin_1[3] = std::abs(b_s[q]);
      varargin_1[4] = std::abs(e[q]);
      i = 1;
      mtmp = varargin_1[0];
      if (rtIsNaN(varargin_1[0])) {
        k = 2;
        exitg2 = false;
        while ((!exitg2) && (k < 6)) {
          i = k;
          if (!rtIsNaN(varargin_1[k - 1])) {
            mtmp = varargin_1[k - 1];
            exitg2 = true;
          } else {
            k++;
          }
        }
      }

      if (i < 5) {
        while (i + 1 < 6) {
          if (varargin_1[i] > mtmp) {
            mtmp = varargin_1[i];
          }

          i++;
        }
      }

      f = b_s[m + 1] / mtmp;
      nrm = b_s[m] / mtmp;
      r = e[m] / mtmp;
      sqds = b_s[q] / mtmp;
      rt = ((nrm + f) * (nrm - f) + r * r) / 2.0;
      nrm = f * r;
      nrm *= nrm;
      if ((rt != 0.0) || (nrm != 0.0)) {
        r = rt * rt + nrm;
        b_sqrt(&r);
        if (rt < 0.0) {
          r = -r;
        }

        r = nrm / (rt + r);
      } else {
        r = 0.0;
      }

      f = (sqds + f) * (sqds - f) + r;
      rt = sqds * (e[q] / mtmp);
      for (k = q + 1; k <= m + 1; k++) {
        xrotg(&f, &rt, &nrm, &r);
        if (k > q + 1) {
          e[0] = f;
        }

        f = nrm * b_s[k - 1] + r * e[k - 1];
        e[k - 1] = nrm * e[k - 1] - r * b_s[k - 1];
        rt = r * b_s[k];
        b_s[k] *= nrm;
        xrot(V, 1 + 3 * (k - 1), 1 + 3 * k, nrm, r);
        b_s[k - 1] = f;
        xrotg(&b_s[k - 1], &rt, &nrm, &r);
        f = nrm * e[k - 1] + r * b_s[k];
        b_s[k] = -r * e[k - 1] + nrm * b_s[k];
        rt = r * e[k];
        e[k] *= nrm;
        xrot(U, 1 + 3 * (k - 1), 1 + 3 * k, nrm, r);
      }

      e[m] = f;
      iter++;
      break;

     default:
      if (b_s[q] < 0.0) {
        b_s[q] = -b_s[q];
        xscal(-1.0, V, 1 + 3 * q);
      }

      i = q + 1;
      while ((q + 1 < 3) && (b_s[q] < b_s[i])) {
        rt = b_s[q];
        b_s[q] = b_s[i];
        b_s[i] = rt;
        xswap(V, 1 + 3 * q, 1 + 3 * (q + 1));
        xswap(U, 1 + 3 * q, 1 + 3 * (q + 1));
        q = i;
        i++;
      }

      iter = 0;
      m--;
      break;
    }
  }

  for (k = 0; k < 3; k++) {
    s[k] = b_s[k];
  }
}

//
// File trailer for svd1.cpp
//
// [EOF]
//

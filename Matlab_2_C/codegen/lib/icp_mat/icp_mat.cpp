//
// File: icp_mat.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "sqrt.h"
#include "combineVectorElements.h"
#include "power.h"
#include "repmat.h"
#include "norm.h"
#include "polyfit.h"
#include "acos.h"
#include "dot.h"
#include "diag.h"
#include "det.h"
#include "svd.h"

// Function Declarations
static void eq_point(const double q[1077], const double p[1077], double weights
                     [359], double R[9], double T[3]);
static double extrapolate(const double v[3], const double d[3], double vmax);
static void match_bruteForce(const double q[1077], const double p[1077], double
  match[359], double mindist[359]);
static void quat2rmat(const double quaternion[4], double R[9]);
static void rmat2quat(const double R[9], double quaternion[4]);
static double rms_error(const double p1[1077], const double p2[1077]);

// Function Definitions

//
// Arguments    : const double q[1077]
//                const double p[1077]
//                double weights[359]
//                double R[9]
//                double T[3]
// Return Type  : void
//
static void eq_point(const double q[1077], const double p[1077], double weights
                     [359], double R[9], double T[3])
{
  double y;
  int i3;
  double q_bar[3];
  double p_bar[3];
  int i4;
  double b_q[1077];
  double dv4[1077];
  double dv5[1077];
  double b_p[1077];
  double c_p[9];
  double U[9];
  double unusedU2[9];
  double V[9];
  int i5;
  double dv6[3];

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  normalize weights
  y = combineVectorElements(weights);
  for (i3 = 0; i3 < 359; i3++) {
    weights[i3] /= y;
  }

  //  find data centroid and deviations from centroid
  //  Apply weights
  //  find data centroid and deviations from centroid
  for (i3 = 0; i3 < 3; i3++) {
    q_bar[i3] = 0.0;
    p_bar[i3] = 0.0;
    for (i4 = 0; i4 < 359; i4++) {
      q_bar[i3] += q[i3 + 3 * i4] * weights[i4];
      p_bar[i3] += p[i3 + 3 * i4] * weights[i4];
    }
  }

  //  Apply weights
  // p_mark = p_mark .* repmat(weights, 3, 1);
  //  taking points of q in matched order
  b_repmat(p_bar, b_q);
  b_repmat(q_bar, dv4);
  c_repmat(weights, dv5);
  for (i3 = 0; i3 < 359; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      b_p[i4 + 3 * i3] = p[i4 + 3 * i3] - b_q[i4 + 3 * i3];
    }
  }

  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 359; i4++) {
      b_q[i4 + 359 * i3] = (q[i3 + 3 * i4] - dv4[i3 + 3 * i4]) * dv5[i3 + 3 * i4];
    }
  }

  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      c_p[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 359; i5++) {
        c_p[i3 + 3 * i4] += b_p[i3 + 3 * i5] * b_q[i5 + 359 * i4];
      }
    }
  }

  svd(c_p, U, unusedU2, V);

  //  singular value decomposition
  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      c_p[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 3; i5++) {
        c_p[i3 + 3 * i4] += U[i3 + 3 * i5] * V[i4 + 3 * i5];
      }
    }
  }

  dv6[0] = 1.0;
  dv6[1] = 1.0;
  dv6[2] = det(c_p);
  diag(dv6, unusedU2);
  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      c_p[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 3; i5++) {
        c_p[i3 + 3 * i4] += V[i3 + 3 * i5] * unusedU2[i5 + 3 * i4];
      }
    }

    y = 0.0;
    for (i4 = 0; i4 < 3; i4++) {
      R[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 3; i5++) {
        R[i3 + 3 * i4] += c_p[i3 + 3 * i5] * U[i4 + 3 * i5];
      }

      y += R[i3 + 3 * i4] * p_bar[i4];
    }

    T[i3] = q_bar[i3] - y;
  }
}

//
// Arguments    : const double v[3]
//                const double d[3]
//                double vmax
// Return Type  : double
//
static double extrapolate(const double v[3], const double d[3], double vmax)
{
  double dv;
  double p1[2];
  double p2[3];
  double v2;
  double x[4];
  boolean_T y;
  int k;
  boolean_T exitg1;
  boolean_T guard1 = false;
  boolean_T p;

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Extrapolation in quaternion space. Details are found in:
  //
  //  Besl, P., & McKay, N. (1992). A method for registration of 3-D shapes.
  //  IEEE Transactions on pattern analysis and machine intelligence, 239?256.
  polyfit(v, d, p1);

  //  linear fit
  b_polyfit(v, d, p2);

  //  parabolic fit
  dv = -p1[1] / p1[0];

  //  linear zero crossing
  v2 = -p2[1] / (2.0 * p2[0]);

  //  polynomial top point
  x[0] = 0.0;
  x[1] = v2;
  x[2] = dv;
  x[3] = vmax;
  y = true;
  k = 1;
  exitg1 = false;
  while ((!exitg1) && (k < 4)) {
    if ((x[k - 1] <= x[k]) || rtIsNaN(x[k])) {
      p = true;
    } else {
      p = false;
    }

    if (!p) {
      y = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  guard1 = false;
  if (y) {
    dv = v2;
  } else {
    x[0] = 0.0;
    x[1] = v2;
    x[2] = vmax;
    x[3] = dv;
    y = true;
    k = 1;
    exitg1 = false;
    while ((!exitg1) && (k < 4)) {
      if ((x[k - 1] <= x[k]) || rtIsNaN(x[k])) {
        p = true;
      } else {
        p = false;
      }

      if (!p) {
        y = false;
        exitg1 = true;
      } else {
        k++;
      }
    }

    if (y) {
      dv = v2;
    } else {
      x[0] = 0.0;
      x[1] = dv;
      x[2] = v2;
      x[3] = vmax;
      y = true;
      k = 1;
      exitg1 = false;
      while ((!exitg1) && (k < 4)) {
        if ((x[k - 1] <= x[k]) || rtIsNaN(x[k])) {
          p = true;
        } else {
          p = false;
        }

        if (!p) {
          y = false;
          exitg1 = true;
        } else {
          k++;
        }
      }

      if (y) {
      } else {
        x[0] = 0.0;
        x[1] = dv;
        x[2] = vmax;
        x[3] = v2;
        y = true;
        k = 1;
        exitg1 = false;
        while ((!exitg1) && (k < 4)) {
          if ((x[k - 1] <= x[k]) || rtIsNaN(x[k])) {
            p = true;
          } else {
            p = false;
          }

          if (!p) {
            y = false;
            exitg1 = true;
          } else {
            k++;
          }
        }

        if (y) {
        } else if (v2 < 0.0) {
          p2[0] = 0.0;
          p2[1] = dv;
          p2[2] = vmax;
          y = true;
          k = 1;
          exitg1 = false;
          while ((!exitg1) && (k < 3)) {
            if ((p2[k - 1] <= p2[k]) || rtIsNaN(p2[k])) {
              p = true;
            } else {
              p = false;
            }

            if (!p) {
              y = false;
              exitg1 = true;
            } else {
              k++;
            }
          }

          if (y) {
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }
  }

  if (guard1) {
    if ((dv > vmax) && (v2 > vmax)) {
      dv = vmax;
    } else {
      dv = 0.0;
    }
  }

  return dv;
}

//
// Arguments    : const double q[1077]
//                const double p[1077]
//                double match[359]
//                double mindist[359]
// Return Type  : void
//
static void match_bruteForce(const double q[1077], const double p[1077], double
  match[359], double mindist[359])
{
  int ki;
  double d[359];
  int ixstart;
  int itmp;
  double mtmp;
  double b_q[359];
  double dv3[359];
  int ix;
  boolean_T exitg1;

  // if not(arg.ReturnAll)
  // 	% 原来这样在Matlab Coder里会报Mismatch，这边只好加入一个临时变量，用于返回 
  //    % 这段也弃用
  //     TR = TR(:,:,end);
  //     TT = TT(:,:,end);
  // end
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  暴力匹配
  for (ki = 0; ki < 359; ki++) {
    mindist[ki] = 0.0;
    memset(&d[0], 0, 359U * sizeof(double));
    for (ixstart = 0; ixstart < 3; ixstart++) {
      for (itmp = 0; itmp < 359; itmp++) {
        b_q[itmp] = q[ixstart + 3 * itmp] - p[ixstart + 3 * ki];
      }

      power(b_q, dv3);
      for (itmp = 0; itmp < 359; itmp++) {
        d[itmp] += dv3[itmp];
      }
    }

    ixstart = 1;
    mtmp = d[0];
    itmp = 1;
    if (rtIsNaN(d[0])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix < 360)) {
        ixstart = ix;
        if (!rtIsNaN(d[ix - 1])) {
          mtmp = d[ix - 1];
          itmp = ix;
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 359) {
      while (ixstart + 1 < 360) {
        if (d[ixstart] < mtmp) {
          mtmp = d[ixstart];
          itmp = ixstart + 1;
        }

        ixstart++;
      }
    }

    mindist[ki] = mtmp;
    match[ki] = itmp;
    mindist[ki] = std::sqrt(mindist[ki]);
  }
}

//
// 这里增加一个初始化，这边敢这么设置是因为这个东西本身就是二维的
// Arguments    : const double quaternion[4]
//                double R[9]
// Return Type  : void
//
static void quat2rmat(const double quaternion[4], double R[9])
{
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Converts (unit) quaternion representations to (orthogonal) rotation matrices R 
  //
  //  Input: A 4xn matrix of n quaternions
  //  Output: A 3x3xn matrix of corresponding rotation matrices
  //
  //  http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#From_a_quaternion_to_an_orthogonal_matrix 
  R[0] = ((quaternion[0] * quaternion[0] + quaternion[1] * quaternion[1]) -
          quaternion[2] * quaternion[2]) - quaternion[3] * quaternion[3];
  R[3] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[0] * quaternion
    [3];
  R[6] = 2.0 * quaternion[1] * quaternion[3] + 2.0 * quaternion[0] * quaternion
    [2];
  R[1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[0] * quaternion
    [3];
  R[4] = ((quaternion[0] * quaternion[0] - quaternion[1] * quaternion[1]) +
          quaternion[2] * quaternion[2]) - quaternion[3] * quaternion[3];
  R[7] = 2.0 * quaternion[2] * quaternion[3] - 2.0 * quaternion[0] * quaternion
    [1];
  R[2] = 2.0 * quaternion[1] * quaternion[3] - 2.0 * quaternion[0] * quaternion
    [2];
  R[5] = 2.0 * quaternion[2] * quaternion[3] + 2.0 * quaternion[0] * quaternion
    [1];
  R[8] = ((quaternion[0] * quaternion[0] - quaternion[1] * quaternion[1]) -
          quaternion[2] * quaternion[2]) + quaternion[3] * quaternion[3];

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

//
// Arguments    : const double R[9]
//                double quaternion[4]
// Return Type  : void
//
static void rmat2quat(const double R[9], double quaternion[4])
{
  double x;
  double b_x;
  double c_x;

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Converts (orthogonal) rotation matrices R to (unit) quaternion
  //  representations
  //
  //  Input: A 3x3xn matrix of rotation matrices
  //  Output: A 4xn matrix of n corresponding quaternions
  //
  //  http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
  x = R[5] - R[7];
  if (x < 0.0) {
    x = -1.0;
  } else if (x > 0.0) {
    x = 1.0;
  } else {
    if (x == 0.0) {
      x = 0.0;
    }
  }

  b_x = R[6] - R[2];
  if (b_x < 0.0) {
    b_x = -1.0;
  } else if (b_x > 0.0) {
    b_x = 1.0;
  } else {
    if (b_x == 0.0) {
      b_x = 0.0;
    }
  }

  c_x = R[1] - R[3];
  if (c_x < 0.0) {
    c_x = -1.0;
  } else if (c_x > 0.0) {
    c_x = 1.0;
  } else {
    if (c_x == 0.0) {
      c_x = 0.0;
    }
  }

  quaternion[0] = 0.5 * std::sqrt(((1.0 + R[0]) + R[4]) + R[8]);
  quaternion[1] = 0.5 * x * std::sqrt(((1.0 + R[0]) - R[4]) - R[8]);
  quaternion[2] = 0.5 * b_x * std::sqrt(((1.0 - R[0]) + R[4]) - R[8]);
  quaternion[3] = 0.5 * c_x * std::sqrt(((1.0 - R[0]) - R[4]) + R[8]);
}

//
// Arguments    : const double p1[1077]
//                const double p2[1077]
// Return Type  : double
//
static double rms_error(const double p1[1077], const double p2[1077])
{
  int i;
  double b_p1[1077];
  double x[1077];
  double dsq[359];
  int xpageoffset;
  int k;

  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Determine the RMS error between two point equally sized point clouds with
  //  point correspondance.
  //  ER = rms_error(p1,p2) where p1 and p2 are 3xn matrices.
  for (i = 0; i < 1077; i++) {
    b_p1[i] = p1[i] - p2[i];
  }

  b_power(b_p1, x);
  for (i = 0; i < 359; i++) {
    xpageoffset = i * 3;
    dsq[i] = x[xpageoffset];
    for (k = 0; k < 2; k++) {
      dsq[i] += x[(xpageoffset + k) + 1];
    }
  }

  return std::sqrt(combineVectorElements(dsq) / 359.0);
}

//
// 输入变量，因为原版的是通过inputParser，这个在Matlab coder中不识别，所以这边略去这个部分，直接给输入变量赋值
// Arguments    : const double q[1077]
//                const double p[1077]
//                double Tt[3]
//                double Tr[9]
//                double ER[16]
//                double *t
// Return Type  : void
//
void icp_mat(const double q[1077], const double p[1077], double Tt[3], double
             Tr[9], double ER[16], double *t)
{
  double pt[1077];
  double TT[48];
  double TR[144];
  int i0;
  double theta[16];
  double qq[112];
  static const signed char iv0[112] = { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };

  double dq[112];
  int k;
  double match[359];
  double mindist[359];
  int i1;
  double dv0[359];
  double b_q[1077];
  double R[9];
  double T[3];
  double dv;
  double b_R[9];
  int i2;
  double c_R[3];
  double dv1[4];
  double y;
  double d0;
  double q_mark[7];
  double dv2[359];

  //  false
  //  迭代次数，默认10次
  //  控制点匹配手法，注意这里只有暴力匹配能被Matlab Coder使用，其他手段都不能用 
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //  Actual implementation 实际应用
  //  Allocate vector for RMS of errors in every iteration.
  //  这个t是结果用来统计均方根误差用的，这里完全可以不要，因为在Matlab Coder内不识别 
  //  t = zeros(arg.iter+1,1);
  *t = 0.0;

  //  只好这里放弃，在C里面再增加值进行测试
  //  Start timer
  //  tic;
  //  Transformed data point cloud
  memcpy(&pt[0], &p[0], 1077U * sizeof(double));

  //  Allocate vector for RMS of errors in every iteration.
  memset(&ER[0], 0, sizeof(double) << 4);

  //  Initialize temporary transform vector and matrix.
  //  Initialize total transform vector(s) and rotation matric(es).
  memset(&TT[0], 0, 48U * sizeof(double));
  repmat(TR);

  //  这两个变量是用于返回的，上面两个是过程变量
  //  之所以这么做是因为Matlab在返回的时候会报Mismatch
  //  If edge vertices should be rejected, find edge vertices
  //  Initialize total transform vector (quaternion ; translation vec.)
  //  Allocate vector for direction change and change angle.
  for (i0 = 0; i0 < 112; i0++) {
    qq[i0] = iv0[i0];
    dq[i0] = 0.0;
  }

  memset(&theta[0], 0, sizeof(double) << 4);

  // t(1) = toc;
  //  Go into main iteration loop
  for (k = 0; k < 15; k++) {
    //  Do matching
    match_bruteForce(q, pt, match, mindist);

    //         case 'Delaunay'
    //             [match mindist] = match_Delaunay(q,pt,DT);
    //         case 'kDtree'
    //             [match mindist] = match_kDtree(q,pt,kdOBJ);
    //  If matches to edge vertices should be rejected
    //  If worst matches should be rejected
    if (1 + k == 1) {
      power(mindist, dv0);
      ER[k] = combineVectorElements(dv0) / 359.0;
      b_sqrt(&ER[k]);
    }

    //  Determine weight vector
    for (i0 = 0; i0 < 359; i0++) {
      for (i1 = 0; i1 < 3; i1++) {
        b_q[i1 + 3 * i0] = q[i1 + 3 * ((int)match[i0] - 1)];
      }

      mindist[i0] = 1.0;
    }

    eq_point(b_q, pt, mindist, R, T);

    //         case 'plane'
    //             weights = arg.Weight(match);
    //             [R,T] = eq_plane(q(:,q_idx),pt(:,p_idx),arg.Normals(:,q_idx),weights(p_idx)); 
    //         case 'lmaPoint'
    //             [R,T] = eq_lmaPoint(q(:,q_idx),pt(:,p_idx));
    //  Add to the total transformation
    for (i0 = 0; i0 < 3; i0++) {
      for (i1 = 0; i1 < 3; i1++) {
        b_R[i0 + 3 * i1] = 0.0;
        for (i2 = 0; i2 < 3; i2++) {
          b_R[i0 + 3 * i1] += R[i0 + 3 * i2] * TR[(i2 + 3 * i1) + 9 * k];
        }
      }
    }

    for (i0 = 0; i0 < 3; i0++) {
      dv = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        TR[(i1 + 3 * i0) + 9 * (k + 1)] = b_R[i1 + 3 * i0];
        dv += R[i0 + 3 * i1] * TT[i1 + 3 * k];
      }

      c_R[i0] = dv + T[i0];
    }

    for (i0 = 0; i0 < 3; i0++) {
      TT[i0 + 3 * (k + 1)] = c_R[i0];
    }

    //  Apply last transformation
    b_repmat(*(double (*)[3])&TT[3 * (k + 1)], b_q);
    for (i0 = 0; i0 < 3; i0++) {
      for (i1 = 0; i1 < 359; i1++) {
        dv = 0.0;
        for (i2 = 0; i2 < 3; i2++) {
          dv += TR[(i0 + 3 * i2) + 9 * (k + 1)] * p[i2 + 3 * i1];
        }

        pt[i0 + 3 * i1] = dv + b_q[i0 + 3 * i1];
      }
    }

    //  Root mean of objective function
    for (i0 = 0; i0 < 359; i0++) {
      for (i1 = 0; i1 < 3; i1++) {
        b_q[i1 + 3 * i0] = q[i1 + 3 * ((int)match[i0] - 1)];
      }
    }

    ER[k + 1] = rms_error(b_q, pt);

    //  If Extrapolation, we might be able to move quicker
    rmat2quat(*(double (*)[9])&TR[9 * (k + 1)], dv1);
    for (i0 = 0; i0 < 4; i0++) {
      qq[i0 + 7 * (k + 1)] = dv1[i0];
    }

    for (i0 = 0; i0 < 3; i0++) {
      qq[(i0 + 7 * (k + 1)) + 4] = TT[i0 + 3 * (k + 1)];
    }

    for (i0 = 0; i0 < 7; i0++) {
      dq[i0 + 7 * (k + 1)] = qq[i0 + 7 * (k + 1)] - qq[i0 + 7 * k];
    }

    dv = dot(*(double (*)[7])&dq[7 * k], *(double (*)[7])&dq[7 * (k + 1)]) /
      (norm(*(double (*)[7])&dq[7 * k]) * norm(*(double (*)[7])&dq[7 * (k + 1)]));
    b_acos(&dv);
    theta[k + 1] = 57.295779513082323 * dv;
    if ((1 + k > 2) && (theta[k + 1] < 10.0) && (theta[k] < 10.0)) {
      dv = norm(*(double (*)[7])&dq[7 * (k + 1)]);
      y = norm(*(double (*)[7])&dq[7 * k]);
      d0 = norm(*(double (*)[7])&dq[7 * (k + 1)]);
      T[0] = 0.0;
      T[1] = -dv;
      T[2] = -y - d0;
      c_R[0] = ER[k + 1];
      c_R[1] = ER[k];
      c_R[2] = ER[k - 1];
      dv = extrapolate(T, c_R, 25.0 * norm(*(double (*)[7])&dq[7 * (k + 1)]));
      if (dv != 0.0) {
        y = norm(*(double (*)[7])&dq[7 * (k + 1)]);
        for (i0 = 0; i0 < 7; i0++) {
          q_mark[i0] = qq[i0 + 7 * (k + 1)] + dv * dq[i0 + 7 * (k + 1)] / y;
        }

        y = b_norm(*(double (*)[4])&q_mark[0]);
        for (i0 = 0; i0 < 4; i0++) {
          q_mark[i0] /= y;
        }

        for (i0 = 0; i0 < 7; i0++) {
          qq[i0 + 7 * (k + 1)] = q_mark[i0];
        }

        quat2rmat(*(double (*)[4])&qq[7 * (k + 1)], *(double (*)[9])&TR[9 * (k +
                   1)]);
        for (i0 = 0; i0 < 3; i0++) {
          TT[i0 + 3 * (k + 1)] = qq[(i0 + 7 * (k + 1)) + 4];
        }

        //  Reapply total transformation
        b_repmat(*(double (*)[3])&TT[3 * (k + 1)], b_q);
        for (i0 = 0; i0 < 3; i0++) {
          for (i1 = 0; i1 < 359; i1++) {
            dv = 0.0;
            for (i2 = 0; i2 < 3; i2++) {
              dv += TR[(i0 + 3 * i2) + 9 * (k + 1)] * p[i2 + 3 * i1];
            }

            pt[i0 + 3 * i1] = dv + b_q[i0 + 3 * i1];
          }
        }

        //  Recalculate root mean of objective function
        //  Note this is costly and only for fun!
        match_bruteForce(q, pt, match, mindist);

        //                     case 'Delaunay'
        //                         [~, mindist] = match_Delaunay(q,pt,DT);
        //                     case 'kDtree'
        //                         [~, mindist] = match_kDtree(q,pt,kdOBJ);
        power(mindist, dv2);
        ER[k + 1] = combineVectorElements(dv2) / 359.0;
        b_sqrt(&ER[k + 1]);
      }
    }

    //  t(k+1) = toc;
  }

  for (i0 = 0; i0 < 3; i0++) {
    for (i1 = 0; i1 < 3; i1++) {
      Tr[i1 + 3 * i0] = TR[135 + (i1 + 3 * i0)];
    }

    Tt[i0] = TT[45 + i0];
  }
}

//
// File trailer for icp_mat.cpp
//
// [EOF]
//

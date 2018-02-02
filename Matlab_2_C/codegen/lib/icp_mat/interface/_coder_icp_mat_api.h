/*
 * File: _coder_icp_mat_api.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 02-Feb-2018 10:03:26
 */

#ifndef _CODER_ICP_MAT_API_H
#define _CODER_ICP_MAT_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_icp_mat_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void icp_mat(real_T q[1077], real_T p[1077], real_T Tt[3], real_T Tr[9],
                    real_T ER[16], real_T *t);
extern void icp_mat_api(const mxArray * const prhs[2], const mxArray *plhs[4]);
extern void icp_mat_atexit(void);
extern void icp_mat_initialize(void);
extern void icp_mat_terminate(void);
extern void icp_mat_xil_terminate(void);

#endif

/*
 * File trailer for _coder_icp_mat_api.h
 *
 * [EOF]
 */

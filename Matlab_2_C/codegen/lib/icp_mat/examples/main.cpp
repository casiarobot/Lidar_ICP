//
// File: main.cpp
//
// MATLAB Coder version            : 3.4
// C/C++ source code generated on  : 02-Feb-2018 10:03:26
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "icp_mat.h"
#include "main.h"
#include "icp_mat_terminate.h"
#include "icp_mat_initialize.h"

// Function Declarations
static void argInit_3x359_real_T(double result[1077]);
static double argInit_real_T();
static void main_icp_mat();

// Function Definitions

//
// Arguments    : double result[1077]
// Return Type  : void
//
static void argInit_3x359_real_T(double result[1077])
{
  int idx0;
  int idx1;

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < 3; idx0++) {
    for (idx1 = 0; idx1 < 359; idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx0 + 3 * idx1] = argInit_real_T();
    }
  }
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_icp_mat()
{
  double dv8[1077];
  double dv9[1077];
  double Tt[3];
  double Tr[9];
  double ER[16];
  double t;

  // Initialize function 'icp_mat' input arguments.
  // Initialize function input argument 'q'.
  // Initialize function input argument 'p'.
  // Call the entry-point 'icp_mat'.
  argInit_3x359_real_T(dv8);
  argInit_3x359_real_T(dv9);
  icp_mat(dv8, dv9, Tt, Tr, ER, &t);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  icp_mat_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_icp_mat();

  // Terminate the application.
  // You do not need to do this more than one time.
  icp_mat_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//

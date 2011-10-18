//	Copyright 2011 Johns Hopkins University
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


/* $Id: turblib.c,v 1.13 2009-10-23 17:58:57 eric Exp $ */
#include <stdio.h>

#include "soapH.h"
#include "TurbulenceServiceSoap.nsmap"
#include "turblib.h"


/* global gSOAP runtime environment
 * Temporary until we can figure out how to pass a pointer
 * to and from a function correctly in Fortran.
 */
struct soap __jhuturbsoap;

/* Error reporting - C */
char __turblib_err[TURB_ERROR_LENGTH];
int __turblib_errno = 0;
int __turblib_exit_on_error = 1;

char * turblibGetErrorString() {
  return __turblib_err;
}

int turblibGetErrorNumber() {
  return __turblib_errno;
}

void turblibPrintError() {
  fprintf(stderr, "%d: %s\n", turblibGetErrorNumber(), turblibGetErrorString());
}

void turblibSetExitOnError(int v) {
  __turblib_exit_on_error = v;
}


/* Error reporting - Fortran */
void turblibgeterrorstring_ (char *dest, int len) {
  strncpy(dest, __turblib_err, len);
}

int turblibgeterrornumber_() {
  return turblibGetErrorNumber();
}

void turblibprinterror_() {
  turblibPrintError();
}

void turblibsetexitonerror_(int *v) {
  turblibSetExitOnError(*v);
}

/* Determine appropriate error behavior */
void turblibHandleError() {
  if (__turblib_exit_on_error) {
    turblibPrintError();
    exit(1);
  }
}


/* Return the enum relating to the Fortran constant */
enum turb1__SpatialInterpolation SpatialIntToEnum(enum SpatialInterpolation spatial)
{
  switch (spatial)
  {
    case 0:
      return turb1__SpatialInterpolation__None;
    case 4:
      return turb1__SpatialInterpolation__Lag4;
    case 6:
      return turb1__SpatialInterpolation__Lag6;
    case 8:
      return turb1__SpatialInterpolation__Lag8;
    case 40:
      return turb1__SpatialInterpolation__None_USCOREFd4;
    case 44:
      return turb1__SpatialInterpolation__Fd4Lag4;
    case 60:
      return turb1__SpatialInterpolation__None_USCOREFd6;
    case 80:
      return turb1__SpatialInterpolation__None_USCOREFd8;
  }
  return -1;
}

/* Return the enum relating to the Fortran constant */
enum turb1__TemporalInterpolation TemporalIntToEnum(enum TemporalInterpolation temporal)
{
  switch (temporal)
  {
    case 0:
      return turb1__TemporalInterpolation__None;
    case 1:
      return turb1__TemporalInterpolation__PCHIP;
  }
  return -1;
}

/* Intialize the gSOAP runtime environment */
void soapinit_() {
  soapinit();
}

void soapinit() {
  soap_init(&__jhuturbsoap);
}

/* Destroy the gSOAP environment */
void soapdestroy_ () {
  soap_destroy(&__jhuturbsoap);
}

void soapdestroy () {
  soap_destroy(&__jhuturbsoap);
}

int getvelocity_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getVelocity (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVelocity (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetVelocity input;
  struct _turb1__GetVelocityResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVelocity(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVelocityResult->Vector3,
      output.GetVelocityResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getvelocityandpressure_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][4],
      int len_a, int len_d)
{
  return getVelocityAndPressure (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVelocityAndPressure (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][4])
{
  int rc;

  struct _turb1__GetVelocityAndPressure input;
  struct _turb1__GetVelocityAndPressureResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVelocityAndPressure (&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVelocityAndPressureResult->Vector3P,
      output.GetVelocityAndPressureResult->__sizeVector3P * sizeof(float) * 4);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getpressurehessian_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][6],
      int len_a, int len_d)
{
  return getPressureHessian(authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getPressureHessian(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][6])
{
  int rc;

  struct _turb1__GetPressureHessian input;
  struct _turb1__GetPressureHessianResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetPressureHessian (&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetPressureHessianResult->PressureHessian,
      output.GetPressureHessianResult->__sizePressureHessian * sizeof(float) * 6);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getvelocitygradient_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][9],
      int len_a, int len_d)
{
  return getVelocityGradient(authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVelocityGradient(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][9])
{
  int rc;

  struct _turb1__GetVelocityGradient input;
  struct _turb1__GetVelocityGradientResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVelocityGradient (&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVelocityGradientResult->Gradient,
      output.GetVelocityGradientResult->__sizeGradient * sizeof(float) * 9);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getpressuregradient_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getPressureGradient(authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getPressureGradient(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetPressureGradient input;
  struct _turb1__GetPressureGradientResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetPressureGradient (&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetPressureGradientResult->Vector3,
      output.GetPressureGradientResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getvelocityhessian_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][18],
      int len_a, int len_d)
{
  return getVelocityHessian(authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVelocityHessian(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][18])
{
  int rc;

  struct _turb1__GetVelocityHessian input;
  struct _turb1__GetVelocityHessianResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVelocityHessian (&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVelocityHessianResult->Hessian,
      output.GetVelocityHessianResult->__sizeHessian * sizeof(float) * 18);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getvelocitylaplacian_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getVelocityLaplacian (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVelocityLaplacian (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetVelocityLaplacian input;
  struct _turb1__GetVelocityLaplacianResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVelocityLaplacian(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVelocityLaplacianResult->Vector3,
      output.GetVelocityLaplacianResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int nullop_ (char *authToken, int *count, 
      float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return nullOp (authToken, *count, 
    datain, dataout);
}

int nullOp (char *authToken, int count, 
      float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__NullOp input;
  struct _turb1__NullOpResponse output;

  input.authToken = authToken;

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__NullOp(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.NullOpResult->Vector3,
      output.NullOpResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

//int getboxfiltervelocity_(char *authToken,
//      char *dataset, float *time,
//      float *filterlength,
//      int *nlayers,
//      int *temporal,
//      int *count, float datain[][3], float dataout[][3],
//      int len_a, int len_d)
//{
//  return getBoxFilterVelocity(authToken,
//    dataset, *time,
//    *filterlength, *nlayers,
//    *temporal,
//    *count, datain, dataout);
//}
//
//int getBoxFilterVelocity(char *authToken,
//      char *dataset, float time,
//      float filterLength, int nlayers,
//      enum TemporalInterpolation temporal,
//      int count, float datain[][3], float dataout[][3])
//{
//  int rc;
//
//  struct _turb1__GetBoxFilterVelocity input;
//  struct _turb1__GetBoxFilterVelocityResponse output;
//
//  input.authToken = authToken;
//  input.dataset = dataset;
//  input.time = time;
//  input.filterlength = filterLength;
//  input.nlayers = nlayers;
//  input.temporalInterpolation = TemporalIntToEnum(temporal);
//
//  struct turb1__ArrayOfPoint3 pointArray;
//  pointArray.__sizePoint3 = count;
//  pointArray.Point3 = (void *)datain;
//  input.points = &pointArray;
//
//  rc = soap_call___turb2__GetBoxFilterVelocity(&__jhuturbsoap, NULL, NULL, &input, &output);
//  if (rc == SOAP_OK) {
//    memcpy(dataout, output.GetBoxFilterVelocityResult->Vector3,
//      output.GetBoxFilterVelocityResult->__sizeVector3 * sizeof(float) * 3);
//    bzero(__turblib_err, TURB_ERROR_LENGTH);
//  } else {
//    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
//    turblibHandleError();
//  }
//  
//  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
//  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */
//
//  __turblib_errno = rc;
//  return rc;
//}
//
//int getboxfilterpressure_(char *authToken,
//      char *dataset, float *time,
//      float *filterlength,
//      int *nlayers,
//      int *temporal,
//      int *count, float datain[][3], float dataout[],
//      int len_a, int len_d)
//{
//  return getBoxFilterPressure(authToken,
//    dataset, *time,
//    *filterlength, *nlayers,
//    *temporal,
//    *count, datain, dataout);
//}
//
//int getBoxFilterPressure(char *authToken,
//      char *dataset, float time,
//      float filterLength, int nlayers,
//      enum TemporalInterpolation temporal,
//      int count, float datain[][3], float dataout[])
//{
//  int rc;
//
//  struct _turb1__GetBoxFilterPressure input;
//  struct _turb1__GetBoxFilterPressureResponse output;
//
//  input.authToken = authToken;
//  input.dataset = dataset;
//  input.time = time;
//  input.filterlength = filterLength;
//  input.nlayers = nlayers;
//  input.temporalInterpolation = TemporalIntToEnum(temporal);
//
//  struct turb1__ArrayOfPoint3 pointArray;
//  pointArray.__sizePoint3 = count;
//  pointArray.Point3 = (void *)datain;
//  input.points = &pointArray;
//
//  rc = soap_call___turb2__GetBoxFilterPressure(&__jhuturbsoap, NULL, NULL, &input, &output);
//  if (rc == SOAP_OK) {
//    memcpy(dataout, output.GetBoxFilterPressureResult->float_,
//      output.GetBoxFilterPressureResult->__sizefloat_ * sizeof(float));
//    bzero(__turblib_err, TURB_ERROR_LENGTH);
//  } else {
//    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
//    turblibHandleError();
//  } 
//  
//  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
//  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */
//
//  __turblib_errno = rc;
//  return rc;
//}
//
//
//int getboxfiltersgsstress_(char *authToken,
//      char *dataset, float *time,
//      float *filterlength,
//      int *nlayers,
//      int *temporal,
//      int *count, float datain[][3], float dataout[][6],
//      int len_a, int len_d)
//{
//  return getBoxFilterSGSStress(authToken,
//    dataset, *time,
//    *filterlength, *nlayers,
//    *temporal,
//    *count, datain, dataout);
//}
//
//int getBoxFilterSGSStress(char *authToken,
//      char *dataset, float time,
//      float filterLength, int nlayers,
//      enum TemporalInterpolation temporal,
//      int count, float datain[][3], float dataout[][6])
//{
//  int rc;
//
//  struct _turb1__GetBoxFilterSGSStress input;
//  struct _turb1__GetBoxFilterSGSStressResponse output;
//
//  input.authToken = authToken;
//  input.dataset = dataset;
//  input.time = time;
//  input.filterlength = filterLength;
//  input.nlayers = nlayers;
//  input.temporalInterpolation = TemporalIntToEnum(temporal);
//
//  struct turb1__ArrayOfPoint3 pointArray;
//  pointArray.__sizePoint3 = count;
//  pointArray.Point3 = (void *)datain;
//  input.points = &pointArray;
//
//  rc = soap_call___turb2__GetBoxFilterSGSStress(&__jhuturbsoap, NULL, NULL, &input, &output);
//  if (rc == SOAP_OK) {
//    memcpy(dataout, output.GetBoxFilterSGSStressResult->SGSStress,
//      output.GetBoxFilterSGSStressResult->__sizeSGSStress * sizeof(float) * 6);
//    bzero(__turblib_err, TURB_ERROR_LENGTH);
//  } else {
//    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
//    turblibHandleError();
//  }
//  
//  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
//  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */
//
//  __turblib_errno = rc;
//  return rc;
//}
//
//int getboxfiltervelocitygradient_(char *authToken,
//      char *dataset, float *time,
//      float *filterlength,
//      int *nlayers,
//      int *temporal,
//      int *count, float datain[][3], float dataout[][9],
//      int len_a, int len_d)
//{
//  return getBoxFilterVelocityGradient(authToken,
//    dataset, *time,
//    *filterlength, *nlayers,
//    *temporal,
//    *count, datain, dataout);
//}
//
//int getBoxFilterVelocityGradient(char *authToken,
//      char *dataset, float time,
//      float filterLength, int nlayers,
//      enum TemporalInterpolation temporal,
//      int count, float datain[][3], float dataout[][9])
//{
//  int rc;
//
//  struct _turb1__GetBoxFilterVelocityGradient input;
//  struct _turb1__GetBoxFilterVelocityGradientResponse output;
//
//  input.authToken = authToken;
//  input.dataset = dataset;
//  input.time = time;
//  input.filterlength = filterLength;
//  input.nlayers = nlayers;
//  input.temporalInterpolation = TemporalIntToEnum(temporal);
//
//  struct turb1__ArrayOfPoint3 pointArray;
//  pointArray.__sizePoint3 = count;
//  pointArray.Point3 = (void *)datain;
//  input.points = &pointArray;
//
//  rc = soap_call___turb2__GetBoxFilterVelocityGradient(&__jhuturbsoap, NULL, NULL, &input, &output);
//  if (rc == SOAP_OK) {
//    memcpy(dataout, output.GetBoxFilterVelocityGradientResult->VelocityGradient,
//      output.GetBoxFilterVelocityGradientResult->__sizeVelocityGradient * sizeof(float) * 9);
//    bzero(__turblib_err, TURB_ERROR_LENGTH);
//  } else {
//    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
//    turblibHandleError();
//  }
//  
//  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
//  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */
//
//  __turblib_errno = rc;
//  return rc;
//}

int getForce(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetForce input;
  struct _turb1__GetForceResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetForce(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetForceResult->Vector3,
      output.GetForceResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;
  return rc;
}

int getforce_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getForce (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getPosition(char *authToken,
  char *dataset, float startTime, float endTime, 
  float dt,
  enum SpatialInterpolation spatial,
  int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetPosition input;
  struct _turb1__GetPositionResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.StartTime = startTime;
  input.EndTime = endTime;
  input.dt = dt;
  input.spatialInterpolation = SpatialIntToEnum(spatial);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetPosition(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetPositionResult->Point3,
      output.GetPositionResult->__sizePoint3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getposition_(char *authToken,
      char *dataset, float *startTime, float *endTime,
	  float *dt,
      int *spatial,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getPosition (authToken,
    dataset, *startTime, *endTime,
	*dt,
    *spatial,
    *count, datain, dataout);
}

int getRawVelocity (char *authToken,
      char *dataset, float time,
	  int X, int Y, int Z, int Xwidth, int Ywidth, int Zwidth, char dataout[])
{
  int rc;

  struct _turb1__GetRawVelocity input;
  struct _turb1__GetRawVelocityResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.X = X;
  input.Y = Y;
  input.Z = Z;
  input.Xwidth = Xwidth;
  input.Ywidth = Ywidth;
  input.Zwidth = Zwidth;

  rc = soap_call___turb2__GetRawVelocity(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetRawVelocityResult->__ptr,
      output.GetRawVelocityResult->__size );
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getmagneticfield_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getMagneticField (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getMagneticField (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetMagneticField input;
  struct _turb1__GetMagneticFieldResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetMagneticField(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetMagneticFieldResult->Vector3,
      output.GetMagneticFieldResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getRawMagneticField (char *authToken,
      char *dataset, float time,
	  int X, int Y, int Z, int Xwidth, int Ywidth, int Zwidth, char dataout[])
{
  int rc;

  struct _turb1__GetRawMagneticField input;
  struct _turb1__GetRawMagneticFieldResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.X = X;
  input.Y = Y;
  input.Z = Z;
  input.Xwidth = Xwidth;
  input.Ywidth = Ywidth;
  input.Zwidth = Zwidth;

  rc = soap_call___turb2__GetRawMagneticField(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetRawMagneticFieldResult->__ptr,
      output.GetRawMagneticFieldResult->__size );
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getvectorpotential_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getVectorPotential (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getVectorPotential (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetVectorPotential input;
  struct _turb1__GetVectorPotentialResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetVectorPotential(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetVectorPotentialResult->Vector3,
      output.GetVectorPotentialResult->__sizeVector3 * sizeof(float) * 3);
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getRawVectorPotential (char *authToken,
      char *dataset, float time,
	  int X, int Y, int Z, int Xwidth, int Ywidth, int Zwidth, char dataout[])
{
  int rc;

  struct _turb1__GetRawVectorPotential input;
  struct _turb1__GetRawVectorPotentialResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.X = X;
  input.Y = Y;
  input.Z = Z;
  input.Xwidth = Xwidth;
  input.Ywidth = Ywidth;
  input.Zwidth = Zwidth;

  rc = soap_call___turb2__GetRawVectorPotential(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetRawVectorPotentialResult->__ptr,
      output.GetRawVectorPotentialResult->__size );
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getpressure_ (char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[],
      int len_a, int len_d)
{
  return getPressure (authToken,
    dataset, *time,
    *spatial, *temporal,
    *count, datain, dataout);
}

int getPressure (char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[])
{
  int rc;

  struct _turb1__GetPressure input;
  struct _turb1__GetPressureResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.spatialInterpolation = SpatialIntToEnum(spatial);
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetPressure(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetPressureResult->Pressure,
      output.GetPressureResult->__sizePressure * sizeof(float));
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

int getRawPressure (char *authToken,
      char *dataset, float time,
	  int X, int Y, int Z, int Xwidth, int Ywidth, int Zwidth, char dataout[])
{
  int rc;

  struct _turb1__GetRawPressure input;
  struct _turb1__GetRawPressureResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.X = X;
  input.Y = Y;
  input.Z = Z;
  input.Xwidth = Xwidth;
  input.Ywidth = Ywidth;
  input.Zwidth = Zwidth;

  rc = soap_call___turb2__GetRawPressure(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetRawPressureResult->__ptr,
      output.GetRawPressureResult->__size );
    bzero(__turblib_err, TURB_ERROR_LENGTH);
  } else {
    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
    turblibHandleError();
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  __turblib_errno = rc;

  return rc;
}

//int getPosition_new2(char *authToken,
//  char *dataset, float startTime, float endTime, 
//  int nt,
//  enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
//  int count, float datain[][3], float dataout[][3])
//{
//  int rc;
//
//  struct _turb1__GetPosition_USCOREnew2 input;
//  struct _turb1__GetPosition_USCOREnew2Response output;
//
//  input.authToken = authToken;
//  input.dataset = dataset;
//  input.StartTime = startTime;
//  input.EndTime = endTime;
//  input.nt = nt;
//  input.spatialInterpolation = SpatialIntToEnum(spatial);
//  input.temporalInterpolation = TemporalIntToEnum(temporal);
//
//  struct turb1__ArrayOfPoint3 pointArray;
//  pointArray.__sizePoint3 = count;
//  pointArray.Point3 = (void *)datain;
//  input.points = &pointArray;
//
//  rc = soap_call___turb2__GetPosition_USCOREnew2(&__jhuturbsoap, NULL, NULL, &input, &output);
//  if (rc == SOAP_OK) {
//    memcpy(dataout, output.GetPosition_USCOREnew2Result->Point3,
//      output.GetPosition_USCOREnew2Result->__sizePoint3 * sizeof(float) * 3);
//    bzero(__turblib_err, TURB_ERROR_LENGTH);
//  } else {
//    soap_sprint_fault(&__jhuturbsoap, __turblib_err, TURB_ERROR_LENGTH);
//    turblibHandleError();
//  }
//  
//  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
//  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */
//
//  __turblib_errno = rc;
//
//  return rc;
//}
//
//int getpositionnew2_(char *authToken,
//      char *dataset, float *startTime, float *endTime,
//	  int *nt,
//      int *spatial, int *temporal,
//      int *count, float datain[][3], float dataout[][3],
//      int len_a, int len_d)
//{
//  return getPosition_new2 (authToken,
//    dataset, *startTime, *endTime,
//	*nt,
//    *spatial, *temporal,
//    *count, datain, dataout);
//}


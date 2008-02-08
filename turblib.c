#include <stdio.h>

#include "soapH.h"
#include "TurbulenceServiceSoap.nsmap"
#include "turblib.h"

/* global gSOAP runtime environment
 * Temporary until we can figure out how to pass a pointer
 * to and from a function correctly in Fortran.
 */
struct soap __jhuturbsoap;

/* Return the enum relating to the Fortran constant */
enum turb1__SpatialInterpolation SpatialIntToEnum(enum SpatialInterpolation spatial)
{
  switch (spatial)
  {
    case 0:
      return turb1__SpatialInterpolation__None;
    case 4:
      return turb1__SpatialInterpolation__Fd4Lag4;
    case 6:
      return turb1__SpatialInterpolation__Lag6;
    case 8:
      return turb1__SpatialInterpolation__Lag8;
    case 40:
      return turb1__SpatialInterpolation__None_USCOREFd4;
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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return 0;
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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return 0;
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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return 0;
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
    memcpy(dataout, output.GetVelocityGradientResult->VelocityGradient,
      output.GetVelocityGradientResult->__sizeVelocityGradient * sizeof(float) * 9);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return 0;
}

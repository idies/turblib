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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
    memcpy(dataout, output.GetVelocityGradientResult->VelocityGradient,
      output.GetVelocityGradientResult->__sizeVelocityGradient * sizeof(float) * 9);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
    memcpy(dataout, output.GetVelocityHessianResult->VelocityHessian,
      output.GetVelocityHessianResult->__sizeVelocityHessian * sizeof(float) * 18);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

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
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return rc;
}

int getboxfiltervelocity_(char *authToken,
      char *dataset, float *time,
      float *filterlength,
      int *nlayers,
      int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d)
{
  return getBoxFilterVelocity(authToken,
    dataset, *time,
    *filterlength, *nlayers,
    *temporal,
    *count, datain, dataout);
}

int getBoxFilterVelocity(char *authToken,
      char *dataset, float time,
      float filterLength, int nlayers,
      enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3])
{
  int rc;

  struct _turb1__GetBoxFilterVelocity input;
  struct _turb1__GetBoxFilterVelocityResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.filterlength = filterLength;
  input.nlayers = nlayers;
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetBoxFilterVelocity(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetBoxFilterVelocityResult->Vector3,
      output.GetBoxFilterVelocityResult->__sizeVector3 * sizeof(float) * 3);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return rc;
}

int getboxfilterpressure_(char *authToken,
      char *dataset, float *time,
      float *filterlength,
      int *nlayers,
      int *temporal,
      int *count, float datain[][3], float dataout[],
      int len_a, int len_d)
{
  return getBoxFilterPressure(authToken,
    dataset, *time,
    *filterlength, *nlayers,
    *temporal,
    *count, datain, dataout);
}

int getBoxFilterPressure(char *authToken,
      char *dataset, float time,
      float filterLength, int nlayers,
      enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[])
{
  int rc;

  struct _turb1__GetBoxFilterPressure input;
  struct _turb1__GetBoxFilterPressureResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.filterlength = filterLength;
  input.nlayers = nlayers;
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetBoxFilterPressure(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetBoxFilterPressureResult->float_,
      output.GetBoxFilterPressureResult->__sizefloat_ * sizeof(float));
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  } 
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return rc;
}


int getboxfiltersgsstress_(char *authToken,
      char *dataset, float *time,
      float *filterlength,
      int *nlayers,
      int *temporal,
      int *count, float datain[][3], float dataout[][6],
      int len_a, int len_d)
{
  return getBoxFilterSGSStress(authToken,
    dataset, *time,
    *filterlength, *nlayers,
    *temporal,
    *count, datain, dataout);
}

int getBoxFilterSGSStress(char *authToken,
      char *dataset, float time,
      float filterLength, int nlayers,
      enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][6])
{
  int rc;

  struct _turb1__GetBoxFilterSGSStress input;
  struct _turb1__GetBoxFilterSGSStressResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.filterlength = filterLength;
  input.nlayers = nlayers;
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetBoxFilterSGSStress(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetBoxFilterSGSStressResult->SGSStress,
      output.GetBoxFilterSGSStressResult->__sizeSGSStress * sizeof(float) * 6);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return rc;
}

int getboxfiltervelocitygradient_(char *authToken,
      char *dataset, float *time,
      float *filterlength,
      int *nlayers,
      int *temporal,
      int *count, float datain[][3], float dataout[][9],
      int len_a, int len_d)
{
  return getBoxFilterVelocityGradient(authToken,
    dataset, *time,
    *filterlength, *nlayers,
    *temporal,
    *count, datain, dataout);
}

int getBoxFilterVelocityGradient(char *authToken,
      char *dataset, float time,
      float filterLength, int nlayers,
      enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][9])
{
  int rc;

  struct _turb1__GetBoxFilterVelocityGradient input;
  struct _turb1__GetBoxFilterVelocityGradientResponse output;

  input.authToken = authToken;
  input.dataset = dataset;
  input.time = time;
  input.filterlength = filterLength;
  input.nlayers = nlayers;
  input.temporalInterpolation = TemporalIntToEnum(temporal);

  struct turb1__ArrayOfPoint3 pointArray;
  pointArray.__sizePoint3 = count;
  pointArray.Point3 = (void *)datain;
  input.points = &pointArray;

  rc = soap_call___turb2__GetBoxFilterVelocityGradient(&__jhuturbsoap, NULL, NULL, &input, &output);
  if (rc == SOAP_OK) {
    memcpy(dataout, output.GetBoxFilterVelocityGradientResult->VelocityGradient,
      output.GetBoxFilterVelocityGradientResult->__sizeVelocityGradient * sizeof(float) * 9);
  } else {
    fprintf(stdout, ">>> Error...\n");
    soap_print_fault(&__jhuturbsoap, stdout);
  }
  
  soap_end(&__jhuturbsoap);  /* remove deserialized data and clean up */
  soap_done(&__jhuturbsoap); /*  detach the gSOAP environment  */

  return rc;
}


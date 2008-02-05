#ifndef TURBLIB_H_
#define TURBLIB_H_

#include "soapH.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define TURBLIB_VERSION "0.0.2"

/* Global gSOAP handle
 * TODO: Figure out how to pass this to/from Fortran
 */
extern struct soap __jhuturbsoap;

/* Spatial Interpolation Options */
enum SpatialInterpolation
{
  NoSpatialInterpolation = 0,
  Lagrangian4thOrder = 4,
  Lagrangian6thOrder = 6,
  Lagrangian8thOrder = 8
};

/* Temporal Interpolation Options */
enum TemporalInterpolation
{
  NoTemporalInterpolation = 0,
  PCHIPInterpolation = 1
};

/* C*/
void soapinit ();
void soapdestroy ();

/* Fortran */
void soapinit_ ();
void soapdestroy_ ();

/* C */
int getVelocity (char *authToken,
  char *dataset, float time,
  enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
  int count, float datain[][3], float dataout[][3]);

/* Fortran */
int getvelocity_ (char *authToken,
  char *dataset, float *time,
  int *spatial, int *temporal,
  int *count, float datain[][3], float dataout[][3],
  int len_a, int len_d);

/* C */
int getVelocityAndPressure (char *authToken,
  char *dataset, float time,
  enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
  int count, float datain[][3], float dataout[][4]);

/* Fortran */
int getvelocityandpressure_ (char *authToken,
  char *dataset, float *time,
  int *spatial, int *temporal,
  int *count, float datain[][3], float dataout[][4],
  int len_a, int len_d);

/* C */
int getPressureHessian(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][6]);

/* Fortran */
int getpressurehessian_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][6],
      int len_a, int len_d);

/* C */
int getvelocitygradient_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][9],
      int len_a, int len_d);

/* Fortran */
int getVelocityGradient(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][9]);


#ifdef  __cplusplus
}
#endif

#endif

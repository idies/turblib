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
  /* Spatial Interpolation Flags for GetVelocity & GetVelocityAndPressure */
  NoSInt = 0, /* No spatial interpolatio */
  Lag4 = 4,   /* 4th order Lagrangian interpolation in space */
  Lag6 = 6,   /* 4th order Lagrangian interpolation in space */
  Lag8 = 8,   /* 4th order Lagrangian interpolation in space */

  /* Spatial Differentiation & Interpolation Flags for GetVelocityGradient & GetPressureGradient */
  FD4NoInt = 40, /* 4th order finite differential scheme for grid values, no spatial interpolation */
  FD6NoInt = 60, /* 6th order finite differential scheme for grid values, no spatial interpolation */
  FD8NoInt = 80, /* 8th order finite differential scheme for grid values, no spatial interpolation */
  FD4Lag4 = 44,  /*  4th order finite differential scheme for grid values, 4th order Lagrangian interpolation in space */


  /* Old names, for backward compatibility */
  NoSpatialInterpolation = 0,
  Lagrangian4thOrder = 4,
  Lagrangian6thOrder = 6,
  Lagrangian8thOrder = 8
};

/* Temporal Interpolation Options */
enum TemporalInterpolation
{
  NoTInt = 0,   /* No temporal interpolation */
  PCHIPInt = 1, /* Piecewise cubic Hermit interpolation in time */


  /* Old names, for backward compatibility */
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

/* C */
int getpressuregradient_(char *authToken,
      char *dataset, float *time,
      int *spatial, int *temporal,
      int *count, float datain[][3], float dataout[][3],
      int len_a, int len_d);

/* Fortran */
int getPressureGradient(char *authToken,
      char *dataset, float time,
      enum SpatialInterpolation spatial, enum TemporalInterpolation temporal,
      int count, float datain[][3], float dataout[][3]);



#ifdef  __cplusplus
}
#endif

#endif

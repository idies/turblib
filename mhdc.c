#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "turblib.h"

/*
 * Turbulence Database sample C client code
 */

#define N 10
#define PI  3.14159265358979323846

int main(int argc, char *argv[]) {

  char * authtoken = "edu.jhu.pha.turbulence.testing-201003";
  char * dataset = "mhd1024coarse";
  enum SpatialInterpolation spatialInterp = Lag8;
  enum TemporalInterpolation temporalInterp = NoTInt;

  int X = 0, Y = 0, Z = 0, Xwidth = 16, Ywidth = 16, Zwidth = 16;
  int components = 3;
  float * rawdata = (float*) malloc(Xwidth*Ywidth*Zwidth*sizeof(float)*components);
  int pressure_components = 1;
  float * rawpressure = (float*) malloc(Xwidth*Ywidth*Zwidth*sizeof(float)*pressure_components);

  int timestep = 199;
  float time = 0.002 * timestep;

  float points[N][3];    /* input of x,y,z */
  float result3[N][3];   /* results of x,y,z */
  float pressure[N];	 /* results for pressure */
  int p;

  float dt = 0.0004F;
  float st = 0.285656F;
  float et = 0.295697F;

  /* Initialize gSOAP */
  soapinit();

  /* Enable exit on error.  See README for details. */
  turblibSetExitOnError(1);

  for (p = 0; p < N; p++) {
	  points[p][0] = (float)rand()/RAND_MAX*2*3.141592F;
	  points[p][1] = (float)rand()/RAND_MAX*2*3.141592F;
	  points[p][2] = (float)rand()/RAND_MAX*2*3.141592F;
  }

  printf("Requesting position at %d points, starting at time %f and ending at time %f...\n", N, st, et);
  getPosition (authtoken, dataset, st, et, dt, spatialInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("Requesting raw velocity data...\n");
  getRawVelocity (authtoken, dataset, time, X, Y, Z, Xwidth, Ywidth, Zwidth, (char*)rawdata);
  for (p = 0; p < Xwidth*Ywidth*Zwidth; p++) {
	  printf("%d: Vx=%f, Vy=%f, Vz=%f\n", p, rawdata[p],  rawdata[p+Xwidth*Ywidth*Zwidth],  rawdata[p+2*Xwidth*Ywidth*Zwidth]);
  }
  
  printf("Requesting velocity at N points...\n");
  getVelocity (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("Requesting raw magnetic field data...\n");
  getRawMagneticField (authtoken, dataset, time, X, Y, Z, Xwidth, Ywidth, Zwidth, (char*)rawdata);
  for (p = 0; p < Xwidth*Ywidth*Zwidth; p++) {
	  printf("%d: Bx=%f, By=%f, Bz=%f\n", p, rawdata[p],  rawdata[p+Xwidth*Ywidth*Zwidth],  rawdata[p+2*Xwidth*Ywidth*Zwidth]);
  }

  printf("Requesting magnetic field at N points...\n");
  getMagneticField (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("Requesting raw vector potential data...\n");
  getRawVectorPotential (authtoken, dataset, time, X, Y, Z, Xwidth, Ywidth, Zwidth, (char*)rawdata);
  for (p = 0; p < Xwidth*Ywidth*Zwidth; p++) {
	  printf("%d: Ax=%f, Ay=%f, Az=%f\n", p, rawdata[p],  rawdata[p+Xwidth*Ywidth*Zwidth],  rawdata[p+2*Xwidth*Ywidth*Zwidth]);
  }
  
  printf("Requesting vector potential at N points...\n");
  getVectorPotential (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("Requesting raw pressure data...\n");
  getRawPressure (authtoken, dataset, time, X, Y, Z, Xwidth, Ywidth, Zwidth, (char*)rawpressure);
  for (p = 0; p < Xwidth*Ywidth*Zwidth; p++) {
	  printf("%d: P=%f\n", p, rawdata[p]);
  }
  
  printf("Requesting pressure at N points...\n");
  getPressure (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, pressure);
  for (p = 0; p < N; p++) {
    printf("%d: %f\n", p, pressure[p]);
  }

  free(rawdata);
  free(rawpressure);
  
  /* Free gSOAP resources */
  soapdestroy();

  return 0;
}

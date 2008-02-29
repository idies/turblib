#include <stdio.h>
#include <float.h>
#include "turblib.h"

/*
 * Turbulence Database sample C client code
 */
int main(int argc, char *argv[]) {

  char * authtoken = "jhu.edu.pha.turbulence.testing-200802";
  char * dataset = "isotropic1024coarse";
  enum SpatialInterpolation spatialInterp = Lag6;
  enum TemporalInterpolation temporalInterp = NoTInt;

  float time = 0.08f;

  
  float points[10][3];    /* input of x,y,z */
  float result3[10][3];   /* results of x,y,z */
  float result4[10][4];   /* results of x,y,z,p */
  float result9[10][9];   /* results of Velocity Gradient queries */
  int p;

  /* Initialize gSOAP */
  soapinit();

  for (p = 0; p < 10; p++) {
    points[p][0] = 0.20 * (p+1);
    points[p][1] = 0.50 * (p+1);
    points[p][2] = 0.15 * (p+1);
  }

  printf("Requesting velocity at 10 points...\n");
  getVelocity (authtoken, dataset, time, spatialInterp, temporalInterp, 10, points, result3);
  for (p = 0; p < 10; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

   printf("Requesting velocity and pressure at 10 points...\n");
   getVelocityAndPressure (authtoken, dataset, time, spatialInterp, temporalInterp, 10, points, result4);
   for (p = 0; p < 10; p++) {
     printf("%d: %f,%f,%f p=%f\n", p, result4[p][0], result4[p][1], result4[p][2], result4[p][3]);
   }

  printf("Requesting velocity gradient at 10 points...\n");
  getVelocityGradient (authtoken, dataset, time, FD4Lag4, temporalInterp, 10, points, result9);
  for (p = 0; p < 10; p++) {
    printf("%d: duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n", p,
      result9[p][0], result9[p][1], result9[p][2],
      result9[p][3], result9[p][4], result9[p][5],
      result9[p][6], result9[p][7], result9[p][8]);
  }

  printf("Requesting pressure gradient at 10 points...\n");
  getPressureGradient (authtoken, dataset, time, FD4Lag4, temporalInterp, 10, points, result3);
  for (p = 0; p < 10; p++) {
    printf("%d: dpdx=%f,dpdy=%f,dpdz=%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  /* Free gSOAP resources */
  soapdestroy();

  return 0;
}

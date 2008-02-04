#include <stdio.h>
#include <float.h>
#include "turblib.h"

/*
 * Turbulence Database sample C client code
 */
int main(int argc, char *argv[]) {



  char * authtoken = "jhu.edu.pha.turbulence.testing-200711";
  char * dataset = "isotropic1024coarse";
  enum SpatialInterpolation spatialInterp = Lagrangian6thOrder;
 // enum TemporalInterpolation temporalInterp = NoTemporalInterpolation;
  enum TemporalInterpolation temporalInterp = PCHIPInterpolation;
  float time = 0.06f;

  
  float points[10][3];    /* input of x,y,z */
  float result3[10][3];  /* results of x,y,z */
  float result4[10][4];   /* results of x,y,z,p */
  float result6[10][6];   /* results of x,y,z,p */
  float result9[10][9];   /* results of x,y,z,p */
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
    printf("%d: %f,%f,%f p=%f\n", p, result4[p][0],  result4[p][1],  result4[p][2],  result4[p][3]);
  }

  printf("Requesting pressure hessian at 10 points...\n");
  getPressureHessian (authtoken, dataset, time, spatialInterp, temporalInterp, 10, points, result6);
  for (p = 0; p < 10; p++) {
    printf("%d: d2pdxdx=%f, d2pdxdy=%f, d2pdxdz=%f, d2pdydy=%f, d2pdydz%f, d2pdzdz=%f\n", p, result6[p][0],  result6[p][1],
      result6[p][2],  result6[p][3],  result6[p][4],  result6[p][5]);
  }

  printf("Requesting velocity gradient at 10 points...\n");
  getVelocityGradient (authtoken, dataset, time, spatialInterp, temporalInterp, 10, points, result9);
  for (p = 0; p < 10; p++) {
    printf("%d: duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n", p, result9[p][0],
      result9[p][1], result9[p][2], result9[p][3], result9[p][4],
      result9[p][5], result9[p][6], result9[p][7], result9[p][8]);
  }


  /* Destroy gSOAP */
  soapdestroy();

  return 0;
}

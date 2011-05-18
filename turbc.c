#include <stdio.h>
#include <float.h>
#include <math.h>
#include "turblib.h"

/*
 * Turbulence Database sample C client code
 */
#define N 10

int main(int argc, char *argv[]) {

  char * authtoken = "edu.jhu.pha.turbulence.testing-201104";
  char * dataset = "isotropic1024coarse";
  enum SpatialInterpolation spatialInterp = Lag6;
  enum TemporalInterpolation temporalInterp = NoTInt;
  enum TemporalInterpolation GetPositionTInt = PCHIPInt;

  int nt = 3;

  float dt = 0.002F;
  int timestep = 182;
  float time = dt * timestep;

  float points[N][3];    /* input of x,y,z */
  float result3[N][3];   /* results of x,y,z */
  float result4[N][4];   /* results of x,y,z,p */
  float result6[N][6];   /* results from Pressure Hessian queries */
  float result9[N][9];   /* results from Velocity Gradient queries */
  float result18[N][18];
  int p;

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
  getPosition (authtoken, dataset, st, et, nt, spatialInterp, GetPositionTInt, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }
  
  printf("\nRequesting velocity at N points...\n");
  getVelocity (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("\nRequesting forcing at N points...\n");
  getForce (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: %f,%f,%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("\nRequesting velocity and pressure at N points...\n");
  getVelocityAndPressure (authtoken, dataset, time, spatialInterp, temporalInterp, N, points, result4);
   for (p = 0; p < N; p++) {
     printf("%d: %f,%f,%f p=%f\n", p, result4[p][0], result4[p][1], result4[p][2], result4[p][3]);
  }

  printf("\nRequesting velocity gradient at N points...\n");
  getVelocityGradient (authtoken, dataset, time, FD4Lag4, temporalInterp, N, points, result9);
  for (p = 0; p < N; p++) {
    printf("%d: duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n", p,
      result9[p][0], result9[p][1], result9[p][2],
      result9[p][3], result9[p][4], result9[p][5],
      result9[p][6], result9[p][7], result9[p][8]);
  }

  printf("\nRequesting velocity hessian at N points...\n");
  getVelocityHessian (authtoken, dataset, time, FD4Lag4, temporalInterp, N, points, result18);
  for (p = 0; p < N; p++) {
    printf("%d: d2uxdxdx=%f, d2uxdxdy=%f, d2uxdxdz=%f, d2uxdydy=%f, d2uxdydz=%f, d2uxdzdz=%f, d2uydxdx=%f, d2uydxdy=%f, d2uydxdz=%f, d2uydydy=%f, d2uydydz=%f, d2uydzdz=%f, d2uzdxdx=%f, d2uzdxdy=%f, d2uzdxdz=%f, d2uzdydy=%f, d2uzdydz=%f, d2uzdzdz=%f\n",
	p,
	result18[p][0], result18[p][1], result18[p][2],
	result18[p][3], result18[p][4], result18[p][5],
	result18[p][6], result18[p][7], result18[p][8],
	result18[p][9], result18[p][10], result18[p][11],
	result18[p][12], result18[p][13], result18[p][14],
	result18[p][15], result18[p][16], result18[p][17]);
  }

  printf("\nRequesting velocity laplacian at N points...\n");
  getVelocityLaplacian (authtoken, dataset, time, FD4Lag4, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: grad2ux=%f, grad2uy=%f, grad2uz=%f\n",
           p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("\nRequesting pressure gradient at N points...\n");
  getPressureGradient (authtoken, dataset, time, FD4Lag4, temporalInterp, N, points, result3);
  for (p = 0; p < N; p++) {
    printf("%d: dpdx=%f,dpdy=%f,dpdz=%f\n", p, result3[p][0],  result3[p][1],  result3[p][2]);
  }

  printf("\nRequesting pressure hessian at N points...\n");
  getPressureHessian(authtoken, dataset, time, FD4Lag4, temporalInterp, N, points, result6);
  for (p = 0; p < N; p++) {
    printf("%d: d2pdxdx=%f,d2pdxdy=%f,d2pdxdz=%f, d2pdydy=%f, d2pdydz=%f, d2pdzdz=%f\n", p,
           result6[p][0],  result6[p][1],  result6[p][2], result6[p][3],  result6[p][4],  result6[p][5]);

  }
  
  
  /* Free gSOAP resources */
  soapdestroy();

  return 0;
}

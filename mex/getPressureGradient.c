#include <string.h>
#include "turblib.h"
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

  mwSize nins = 7;
  mwSize nouts = 1;
  char * authkey;
  char * dataset;
  mwSize spatialInterp;
  mwSize temporalInterp;
  float time;
  mwSize count;
  double *mat_input;

  char turblibErrMsg[TURB_ERROR_LENGTH];

    /* check for correct number of input and output arguments */
  if (nrhs !=nins) {
    sprintf(turblibErrMsg, "Required number of input arguments: %d",nins);
    mexErrMsgTxt(turblibErrMsg);
  } else if (nlhs !=nouts) {
    sprintf(turblibErrMsg, "Required number of output arguments: %d",nouts);
    mexErrMsgTxt(turblibErrMsg);
  }
  
  authkey = mxArrayToString(prhs[0]);
  dataset = mxArrayToString(prhs[1]);
  time    = mxGetScalar(prhs[2]);
  spatialInterp   = (int)mxGetScalar(prhs[3]);
  temporalInterp  = (int)mxGetScalar(prhs[4]);
  count   = (int)mxGetScalar(prhs[5]);
  mat_input = mxGetPr(prhs[6]);

  mwSize i,j;
  mwSize nrow, ncol, nrow_out, ncol_out;

  /* Row-major order */
  nrow = mxGetN(prhs[6]);
  ncol = 3;

  nrow_out = nrow;
  ncol_out = 3;
  
  float input[nrow][ncol];
  float output[nrow_out][ncol_out];  
  
  /* Initialize gSOAP */
  soapinit();

  /* Transform data to correct shape */
  for(i=0;i<nrow;i++)
  {
    for(j=0;j<ncol;j++)
    {
        input[i][j] = mat_input[i*ncol + j];
    }
  }

  /* Create output matrix; Column-major order */
  plhs[0] = mxCreateNumericMatrix(ncol_out,nrow_out,mxSINGLE_CLASS,mxREAL);

  /*  Call soap function */
  if (getPressureGradient(authkey, dataset, time, spatialInterp, temporalInterp, count, input, output) != SOAP_OK) {
    sprintf(turblibErrMsg,"%d: %s\n", turblibGetErrorNumber(), turblibGetErrorString());
    mexErrMsgTxt(turblibErrMsg);
  }

  memcpy(mxGetPr(plhs[0]), output, nrow_out*ncol_out*sizeof(float));

  soapdestroy(); 

  return;
}

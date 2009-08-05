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

  authkey = mxArrayToString(prhs[0]);
  dataset = mxArrayToString(prhs[1]);
  time    = mxGetScalar(prhs[2]);
  spatialInterp   = (int)mxGetScalar(prhs[3]);
  temporalInterp  = (int)mxGetScalar(prhs[4]);
  count   = (int)mxGetScalar(prhs[5]);
  mat_input = mxGetPr(prhs[6]);

  mwSize i,j,rc;
  mwSize nrow, ncol, nrow_out, ncol_out;

  /* Row-major order */
  nrow = mxGetN(prhs[6]);
  ncol = 3;

  nrow_out = nrow;
  ncol_out = 6;
  
  float input[nrow][ncol];
  float output[nrow_out][ncol_out];  

  /* check for correct number of input and output arguments */
  if (nrhs !=nins)
    mexErrMsgTxt("Must have seven input arguments");
  if (nlhs !=nouts)
    mexErrMsgTxt("Must have one output argument");
  
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
 rc = getPressureHessian(authkey, dataset, time, spatialInterp, temporalInterp, count, input, output);
 
/*  Check status of request */
 if (rc != SOAP_OK) {
   mexErrMsgTxt("Error with data request! Please check inputs.");
   soapdestroy();
 }

 memcpy(mxGetPr(plhs[0]), output, nrow_out*ncol_out*sizeof(float));

 soapdestroy(); 

 return;
}

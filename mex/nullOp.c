#include <string.h>
#include <turblib.h>
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

  mwSize nins = 3;
  mwSize nouts = 1;
  char * authkey;
  mwSize count;
  double *mat_input;

  authkey = mxArrayToString(prhs[0]);
  count   = (int)mxGetScalar(prhs[1]);
  mat_input = mxGetPr(prhs[2]);

  mwSize i,j,rc;
  mwSize nrow, ncol;

  /* Row-major order */
  nrow = mxGetN(prhs[2]);
  ncol = 3;

  float input[nrow][ncol];
  float output[nrow][ncol];

  /* check for correct number of input and output arguments */
  if (nrhs !=nins)
    mexErrMsgTxt("Must have three input arguments");
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
  plhs[0] = mxCreateNumericMatrix(ncol,nrow,mxSINGLE_CLASS,mxREAL);

  /*  Call soap function */
  rc = nullOp (authkey, count, input, output);
 
  /*  Check status of request */
  if (rc != SOAP_OK) {
    soapdestroy();
    mexErrMsgTxt("Error with data request! Please check inputs.");
  }
 
  memcpy(mxGetPr(plhs[0]), output, nrow*ncol*sizeof(float));

  soapdestroy(); 

  return;
}

#include <string.h>
#include <turblib.h>
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

  mwSize nins = 3;
  mwSize nouts = 1;
  
  char turblibErrMsg[TURB_ERROR_LENGTH];

    /* check for correct number of input and output arguments */
  if (nrhs !=nins) {
    sprintf(turblibErrMsg, "Required number of input arguments: %d",nins);
    mexErrMsgTxt(turblibErrMsg);
  } else if (nlhs !=nouts) {
    sprintf(turblibErrMsg, "Required number of output arguments: %d",nouts);
    mexErrMsgTxt(turblibErrMsg);
  }  

  char * authkey;
  mwSize count;
  double *mat_input;

  authkey = mxArrayToString(prhs[0]);
  count   = (int)mxGetScalar(prhs[1]);
  mat_input = mxGetPr(prhs[2]);

  mwSize i,j;
  mwSize nrow, ncol;

  /* Row-major order */
  nrow = mxGetN(prhs[2]);
  ncol = 3;

  float input[nrow][ncol];
  float output[nrow][ncol];
  
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
  if (nullOp (authkey, count, input, output) != SOAP_OK) {
    sprintf(turblibErrMsg,"%d: %s\n", turblibGetErrorNumber(), turblibGetErrorString());
    mexErrMsgTxt(turblibErrMsg);
  }
 
  memcpy(mxGetPr(plhs[0]), output, nrow*ncol*sizeof(float));

  soapdestroy(); 

  return;
}

#include "mex.h"
#include "math.h"
#include "float.h"
#include "matrix.h"

/*
 * VestBMS_c1postandlikec1qtrapz_mat_mex.c
 *
 *VESTBMS_C1POSTANDLIKEC1QTRAPZ Multiple computations for C=1 (uncorrelated) 
 *
 * ================ INPUT VARIABLES ====================
 * POSTPDF_C2: p(s) * p(x_vest|s). [S-by-1-by-K] (double)
 * LIKE_VIS: p(x_vis|s). [S-by-K] (double)
 *
 * ================ OUTPUT VARIABLES ==================
 * POSTRIGHT_C1: p(right|x_vis,x_vest,C=1). [K-by-K] (double)
 * LIKEC1: p(x_vis,x_vest|C=1). [K-by-K] (double)
 *
 * This is a MEX-file for MATLAB.
 * Template C code generated on 21-Aug-2016 with MEXXER v0.2 
 * (https://github.com/lacerbi/mexxer).
 */

/* Set ARGSCHECK to 0 to skip argument checking (for minor speedup) */
#define ARGSCHECK 1

void VestBMS_c1postandlikec1qtrapz( double *postright_c1, double *likec1, double *postpdf_c2, double *like_vis, mwSize K, mwSize S )
{
	
    mwSize i,j,k,i0deg;
    double lsum,rsum,pmin,*x0_a,*x0_b;
    
    /* Index of 0 deg */
    i0deg = (S+1)/2; 
    
    /* Add some small probability to prevent 0 / 0 */
    pmin = 0.5 * DBL_MIN * (double) S;
    
    /* copy base pointers */    
    x0_a = postpdf_c2;
    x0_b = like_vis;
    
    for (k=0; k<K; k++) {
        for (j=0; j<K; j++) {
            postpdf_c2 = (x0_a + j*S);
            like_vis = (x0_b + k*S);
            
            lsum = 0.5 * *(postpdf_c2++) * *(like_vis++);
            for (i=1; i<i0deg-1; i++) {
                lsum += *(postpdf_c2++) * *(like_vis++);
            }
            lsum += 0.5 * *(postpdf_c2) * *(like_vis); /* last row, do not advance */
            
            rsum = 0.5 * *(postpdf_c2++) * *(like_vis++);
            for (i=i0deg; i<S-1; i++) {
                rsum += *(postpdf_c2++) * *(like_vis++);
            }
            rsum += 0.5 * *(postpdf_c2) * *(like_vis);

            postright_c1[j*K+k] = (rsum + pmin) / (rsum + lsum + pmin);
            likec1[j*K+k] = rsum + lsum;
        }
    }
    	
}

/* the gateway function */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )
{
	double *postright_c1, *likec1, *postpdf_c2, *like_vis;
#if ( ARGSCHECK==0 )
	mwSize *dims_postpdf_c2, *dims_like_vis;
#else /* ( ARGSCHECK!=0 ) */ 
	mwSize *dims_postpdf_c2, *dims_like_vis;
#endif /* ( ARGSCHECK!=0 ) */ 
	mwSize K, S;

	/*  check for proper number of arguments */
	/* NOTE: You do not need an else statement when using mexErrMsgIdAndTxt
	   within an if statement, because it will never get to the else
	   statement if mexErrMsgIdAndTxt is executed. (mexErrMsgIdAndTxt breaks
	   you out of the MEX-file) */
	if ( nrhs<2 || nrhs>2 )
		mexErrMsgIdAndTxt( "MATLAB:VestBMS_c1postandlikec1qtrapz:invalidNumInputs",
			"Two inputs required.");
	if ( nlhs<2 || nlhs>2 )
		mexErrMsgIdAndTxt( "MATLAB:VestBMS_c1postandlikec1qtrapz:invalidNumOutputs",
			"Two outputs required.");

	/* Get first input (POSTPDF_C2, S-by-1-by-K double) */
	postpdf_c2 = (double*) mxGetPr(prhs[0]);
	dims_postpdf_c2 = (mwSize*) mxGetDimensions(prhs[0]);
	S = dims_postpdf_c2[0];
	K = dims_postpdf_c2[2];

	/* Get second input (LIKE_VIS, S-by-K double) */
	like_vis = (double*) mxGetPr(prhs[1]);
	dims_like_vis = (mwSize*) mxGetDimensions(prhs[1]);

	/* Check sizes of input arguments (define ARGSCHECK to 0 above to skip) */
#if ( ARGSCHECK==1 )
		if ( !mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) )
				mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:postpdf_c2NotReal", "Input POSTPDF_C2 must be real.");
		if ( dims_postpdf_c2[0] != ((mwSize) (S)) )
			mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:postpdf_c2WrongSize", "The first dimension of input POSTPDF_C2 has the wrong size (should be S).");
		if ( dims_postpdf_c2[1] != ((mwSize) (1)) )
			mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:postpdf_c2WrongSize", "The second dimension of input POSTPDF_C2 has the wrong size (should be 1).");
		if ( dims_postpdf_c2[2] != ((mwSize) (K)) )
			mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:postpdf_c2WrongSize", "The third dimension of input POSTPDF_C2 has the wrong size (should be K).");

		if ( !mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]) )
				mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:like_visNotReal", "Input LIKE_VIS must be real.");
		if ( dims_like_vis[0] != ((mwSize) (S)) )
			mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:like_visWrongSize", "The first dimension of input LIKE_VIS has the wrong size (should be S).");
		if ( dims_like_vis[1] != ((mwSize) (K)) )
			mexErrMsgIdAndTxt("MATLAB:VestBMS_c1postandlikec1qtrapz:like_visWrongSize", "The second dimension of input LIKE_VIS has the wrong size (should be K).");
#endif /* ( ARGSCHECK==1 ) */ 

	/* Pointer to first output (POSTRIGHT_C1, K-by-K double) */
	plhs[0] = mxCreateDoubleMatrix((mwSize) (K), (mwSize) (K), mxREAL);
	postright_c1 = mxGetPr(plhs[0]);

	/* Pointer to second output (LIKEC1, K-by-K double) */
	plhs[1] = mxCreateDoubleMatrix((mwSize) (K), (mwSize) (K), mxREAL);
	likec1 = mxGetPr(plhs[1]);

	/* Call the C subroutine */
	VestBMS_c1postandlikec1qtrapz(postright_c1, likec1, postpdf_c2, like_vis, K, S);

}

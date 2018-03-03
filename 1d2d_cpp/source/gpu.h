#ifndef _GPU_H_
#define _GPU_H_

namespace GPU_interface_routines
{	

	void TDsolve( int calculations_per_loop, int n_systems,
                            double *ld, 
                            double *dd, 
                                  double *ud,      
                                  double *fin, int device);
	
	void calc_fieldxdf(	int numx, int nump, int numdist,
						double *fin, 
	                    // double *dxf, double *dvf, 
	                    // double *ex, double *vtemp, 
	                    int device);
};


#endif // #ifndef _GPU_H_
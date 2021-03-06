/*! \brief Numerical Methods - Declarations
* \author PICKSC
 * \date   September 1, 2016
 * \file   nmethods.h
 * 
 * This cpp file contains the definitions for the functions
 * required for the numerical methods. 
 */

#ifndef NUMERICAL_METHODS_H
#define NUMERICAL_METHODS_H


//------------------------------------------------------------------------------
/// @brief      Performs Gauss-Seidel method on Ax = b
///
/// @param      A     Input matrix that contains collisional integrals and coefficients
/// @param      b     Vector representing distribution function at current time-step 
/// @param      xk    The solution vector 
///
/// @return     The solution vector representing the distribution function at the next time-step
/// 
///  Fills solution into xk. The other matrices are not modified
///  The function returns "false" if the matrix A is not diagonally
///  dominant
bool Gauss_Seidel(Array2D<double>& A,
                  valarray<complex<double> >& b,
                  valarray<complex<double> >& xk);
bool Gauss_Seidel(Array2D<double>& A,
                  valarray<double >& b,
                  valarray<double >& xk);
//-------------------------------------------------------------------

//-------------------------------------------------------------------

/**
 * @brief      Set up tridiagonal solver
 *
 * @param[in]  a     ???
 * @param[in]  b     ???
 * @param      c     ???
 * @param[in]  d     right side
 * @param      x     solution
 */

void TridiagonalSolve (valarray<double>& a,
                       valarray<double>& b,
                       valarray<double>& c,
                       valarray<complex<double> >&  d,
                       valarray<complex<double> >& x);

void TridiagonalSolve ( size_t calculations_per_loop,
                        valarray<double>& a,
                       valarray<double>& b,
                       valarray<double>& c,
                       valarray<complex<double> >&  d,
                       valarray<complex<double> >& x);

void TridiagonalSolve ( size_t calculations_per_loop,
                        valarray<double>& a,
                        valarray<double>& b,
                        valarray<double>& c,
                        valarray<double>& d,
                        valarray<double>& x);

void TridiagonalSolve (valarray<double>& a,
                       valarray<double>& b,
                       valarray<double>& c,
                       valarray<double>& d,
                       valarray<double>& x);
//-------------------------------------------------------------------


//------------------------------------------------------------------------------
/// @brief      The tridiagonal solver for implicit collisions
///
/// @param      A     Input matrix 
/// @param      d     Right Side
/// @param      xk    Solution vector
///
/// @return     xk is returned as the solution vector
///
///
bool Thomas_Tridiagonal(Array2D<double>& A,
                        valarray<double>& d,
                        valarray<double>& xk);
bool Thomas_Tridiagonal(Array2D<double>& A,
                        valarray<complex<double> >& d,
                        valarray<complex<double> >& xk);
//-------------------------------------------------------------------




//-------------------------------------------------------------------
complex<double> Det33(Array2D<complex <double> >& A);
//-------------------------------------------------------------------
complex<double> Detx33(valarray<complex <double> >& D,
                       Array2D<complex <double> >& A);
//-------------------------------------------------------------------

//-------------------------------------------------------------------
complex<double> Dety33(valarray<complex <double> >& D,
                       Array2D<complex <double> >& A);
//-------------------------------------------------------------------

//-------------------------------------------------------------------
complex<double> Detz33(valarray<complex <double> >& D,
                       Array2D<complex <double> >& A);
//-------------------------------------------------------------------


// Convert double to double
vector<double>    valtovec(const valarray<double>& vDouble);
vector<double>    vdouble_real(const vector<complex<double> >& vDouble);
vector<double>    vdouble_imag(const vector<complex<double> >& vDouble);



#endif

/*! \brief %Algorithms
 * \author PICKSC
 * \date   March 2017
 * \file   lib-algorithms.h
 * 
 * This file includes
 * - Algorithms namespace that contains MakeAxis and Axis
 * - AxisBundle which contains global axes information
 * - %algorithms to calculate Legendre polynomials
 * - moments of the distribution function
 * - RK2, RK3, and RK4 definitions
 */
//--------------------------------------------------------------
#ifndef ALGORITHM_LIBRARY_H
#define ALGORITHM_LIBRARY_H

//**************************************************************
namespace Algorithms {
#pragma optimize("", off)
//--------------------------------------------------------------
    template<typename T>
    valarray<T> MakeAxis(const T min, const T max, const size_t N){
        valarray<T> v(N);
        for (size_t i(0); i < N; ++i) {
            v[i] = static_cast<T>(i);
        }
        v *= (max-min)/(static_cast<T>(N-1));
        v += min;
        return v;
    }

    template<typename T>
    valarray<T> MakeCAxis(const T min, const T max, const size_t N){
        valarray<T> v(N);
        for (size_t i(0); i < N; ++i) {
            v[i] = static_cast<T>(i);
        }
        v *= (max-min)/(static_cast<T>(N));
        v += min+0.5*(max-min)/(static_cast<T>(N));
        return v;
    }

    template<typename T>
    valarray<T> MakeCAxis(const T min, const T max, const T delta){
        size_t N = (max-min)/delta;
        valarray<T> v(N);
        for (size_t i(0); i < N; ++i) {
            v[i] = static_cast<T>(i);
        }
        v *= delta;
        v += min+0.5*delta;
        return v;
    }

    template<typename T>
    valarray<T> MakeCAxis(const T min, const valarray<T> delta_grid)
    {
        size_t N(delta_grid.size());
        valarray<T> v(N);
        v[0] = min + 0.5*delta_grid[0];

        for (size_t i(1); i < N; ++i) {
            v[i]  = delta_grid[i-1];
            v[i] += delta_grid[i];
            v[i] *= 0.5;
            v[i] += v[i-1];
            // std::cout << "\n vR[" << i << "] = " << v[i] << std::endl;
        }
        return v;
    }

    template<typename T> class Axis {
    public:
        valarray<T>* v;

//  Constructor
        Axis(const T min=0, const T max=0, const size_t N=1){
            if (N > 1) {
                v = new valarray<T>(MakeAxis(min,max,N));
            }
            else v = NULL;
        }
//  Copy constructor

        Axis(const Axis& other){
            if (other.v != NULL) {
                v = new valarray<T>(*(other.v));
            }
            else v = NULL;
        }

//  Destructor 
        ~Axis() { delete v; }
    };

    template<typename T> class CAxis {
    public:
        valarray<T>* v;

//  Constructor
        CAxis(const T min=0, const T max=0, const size_t N=1){
            if (N > 1) {
                v = new valarray<T>(MakeCAxis(min,max,N));
            }
            else v = NULL;
        }
        CAxis(const T min=0, const T max=0, const T delta=1){
            size_t N = (max-min)/delta;
            if (N > 1) {
                v = new valarray<T>(MakeCAxis(min,max,delta));
            }
            else v = NULL;
        }
        CAxis(const T min=0, const valarray<T> delta_grid=1){
            if (delta_grid.size() > 1) {
                v = new valarray<T>(MakeCAxis(min,delta_grid));
            }
            else v = NULL;
        }

//  Copy constructor

        CAxis(const CAxis& other){
            if (other.v != NULL) {
                v = new valarray<T>(*(other.v));
            }
            else v = NULL;
        }

//  Destructor 
        ~CAxis() { delete v; }
    };

    template<typename T> class AxisBundle {
    public:
//  Local  axes x(0) --> x, x(1) --> y, x(2) --> z            
        valarray<T> x(size_t i)     const { return *(_x[i].v); }
        size_t      Nx(size_t i)    const  { return (*(_x[i].v)).size(); }
        T           xmin(size_t i)  const  { return (*(_x[i].v))[0]-0.5*_dx[i]; }
        T           xmax(size_t i)  const  { return (*(_x[i].v))[Nx(i)-1]+0.5*_dx[i]; }
        size_t      xdim()          const  { return _x.size();  }
        T           dx(size_t i)    const  { return _dx[i]; }

//  Global axes xg(0) --> x, xg(1) --> y, xg(2) --> z            
        valarray<T> xg(size_t i)    const  { return *(_xg[i].v); }
        size_t      Nxg(size_t i)   const  { return (*(_xg[i].v)).size(); }
        T           xgmin(size_t i) const  { return (*(_xg[i].v))[0]-0.5*_dx[i]; }
        T           xgmax(size_t i) const  { return (*(_xg[i].v))[Nxg(i)-1]+0.5*_dx[i]; }
        size_t      xgdim()         const  { return _xg.size();  }

//  Global axes p(0) --> species1, p(1) --> species2 ...
        valarray<T> p(size_t i)     const  { return *(_p[i].v); }
        size_t      Np(size_t i)    const  { return (*(_p[i].v)).size(); }
        T           pmin(size_t i)  const  { return (*(_p[i].v))[0]-0.5*_dp[i][0]; }
        T           pmax(size_t i)  const  { return (*(_p[i].v))[Np(i)-1]+0.5*_dp[i][Np(i)-1]; }
        size_t      pdim()          const  { return _p.size();  }
        // T           dp(size_t i)    const  { return _dp[i]; }
        // Non-uniform grid
        valarray<T> dp(size_t i)    const  { return (_dp[i]); }

//  Global axes px(0) --> species1, px(1) --> species2 ...
        valarray<T> px(size_t i)    const  { return *(_px[i].v); }
        size_t      Npx(size_t i)   const  { return (*(_px[i].v)).size(); }
        T           pxmin(size_t i) const  { return (*(_px[i].v))[0]-0.5*_dpx[0]; }
        T           pxmax(size_t i) const  { return (*(_px[i].v))[Npx(i)-1]+0.5*_dpx[i][Npx(i)-1]; }
        size_t      pxdim()         const  { return _px.size();  }
        valarray<T> dpx(size_t i)    const  { return (_dpx[i]); }

        //  Global axes py(0) --> species1, py(1) --> species2 ...
        valarray<T> py(size_t i)    const  { return *(_py[i].v); }
        size_t      Npy(size_t i)   const  { return (*(_py[i].v)).size(); }
        T           pymin(size_t i) const  { return (*(_py[i].v))[0]-0.5*_dpy[0]; }
        T           pymax(size_t i) const  { return (*(_py[i].v))[Npy(i)-1]+0.5*_dpy[i][Npy(i)-1]; }
        size_t      pydim()         const  { return _py.size();  }
        valarray<T> dpy(size_t i)    const  { return (_dpy[i]); }

        //  Global axes py(0) --> species1, py(1) --> species2 ...
        valarray<T> pz(size_t i)    const  { return *(_pz[i].v); }
        size_t      Npz(size_t i)   const  { return (*(_pz[i].v)).size(); }
        T           pzmin(size_t i) const  { return (*(_pz[i].v))[0]-0.5*_dpz[0];; }
        T           pzmax(size_t i) const  { return (*(_pz[i].v))[Npz(i)-1]+0.5*_dpz[i][Npz(i)-1]; }
        size_t      pzdim()         const  { return _pz.size();  }
        valarray<T> dpz(size_t i)    const  { return (_dpz[i]); }

//  Constructor
        AxisBundle( const vector<T> _xmin,  const vector<T> _xmax,  const vector<size_t> _Nx,     // local  spatial axes
                    const vector<T> _xgmin, const vector<T> _xgmax, const vector<size_t> _Nxg,    // global spatial axes
                    const vector<valarray<T> > _deltap, 
                    const vector<valarray<T> > _deltapx, const vector<valarray<T> > _deltapy, const vector<valarray<T> > _deltapz){     // momentum axis for each species
                    // dimensions in configuration space
            
            
            for (size_t i(0); i < _Nx.size(); ++i) {
                // std::cout << "\n xmin = " << _xmin[i] << " \n ";
                // std::cout << "\n xmax = " << _xmax[i] << " \n ";
                // std::cout << "\n Nx = " << _Nx[i] << " \n ";
                _x.push_back( CAxis<T>( _xmin[i], _xmax[i], _Nx[i]));
            }
            // std::cout << "\n _Nxg.size() = " << _Nxg.size() << " \n ";
            for (size_t i(0); i < _Nxg.size(); ++i) {
                // std::cout << "\n xgmin = " << _xgmin[i] << " \n ";
                // std::cout << "\n xgmax = " << _xgmax[i] << " \n ";
                // std::cout << "\n Nxg = " << _Nxg[i] << " \n ";
                _xg.push_back( CAxis<T>( _xgmin[i], _xgmax[i], _Nxg[i]));
            }
            for (size_t i(0); i < _Nxg.size(); ++i) {

                _dx.push_back((_xmax[i]-_xmin[i])/(static_cast<T>(_Nx[i])));

            }
            // for (size_t i(0); i < _Np.size(); ++i) {
            //     _p.push_back( Axis<T>( _pmax[i]/(static_cast<T>(2 * _Np[i])), _pmax[i], _Np[i]) );
            // }
            for (size_t i(0); i < _deltap.size(); ++i) {
                // _p.push_back( CAxis<T>(static_cast<T>(0.0), _pmax[i], _Np[i]) );
                // Non-uniform grid
                _p.push_back( CAxis<T>(static_cast<T>(0.0), _deltap[i]) );
            }

            for (size_t i(0); i < _deltap.size(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dp.push_back(_deltap[i]);
            }

            for (size_t i(0); i < _deltapx.size(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpx.push_back(_deltapx[i]);
            }

            for (size_t i(0); i < _deltapy.size(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpy.push_back(_deltapy[i]);
            }

            for (size_t i(0); i < _deltapz.size(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpz.push_back(_deltapz[i]);
            }

            for (size_t i(0); i < _deltapx.size(); ++i) {
                // _px.push_back( Axis<T>( static_cast<T>(-1.0) * _pmax[i], _pmax[i], _Npx[i]));
                // _px.push_back( Axis<T>( static_cast<T>(-1.0) * pmax(i), pmax(i), _Npx[i]));
                _px.push_back( CAxis<T>( static_cast<T>(-1.0) * pmax(i), _deltapx[i]));
            }
            for (size_t i(0); i < _deltapy.size(); ++i) {
                // _py.push_back( Axis<T>( static_cast<T>(-1.0) * _pmax[i], _pmax[i], _Npy[i]));
                // _py.push_back( Axis<T>( static_cast<T>(-1.0) * pmax(i), pmax(i), _Npy[i]));
                _py.push_back( CAxis<T>( static_cast<T>(-1.0) * pmax(i), _deltapy[i]));
            }
            for (size_t i(0); i < _deltapz.size(); ++i) {
                // _pz.push_back( Axis<T>( static_cast<T>(-1.0) * _pmax[i], _pmax[i], _Npz[i]));
                // _pz.push_back( Axis<T>( static_cast<T>(-1.0) * pmax(i), pmax(i), _Npz[i]));
                _pz.push_back( CAxis<T>( static_cast<T>(-1.0) * pmax(i), _deltapz[i]));
            }
        }

        AxisBundle(const AxisBundle& a){
            // std::cout << "\n 12 \n ";
            for (size_t i(0); i < a.xdim(); ++i) {
                // std::cout << "\n a.xmin = " << a.xmin(i) << " \n ";
                // std::cout << "\n a.xmax = " << a.xmax(i) << " \n ";
                // std::cout << "\n a.Nx = " << a.Nx(i) << " \n ";
                _x.push_back( CAxis<T>(a.xmin(i), a.xmax(i), a.Nx(i)) );
            }

            for (size_t i(0); i < a.xgdim(); ++i) {
                // std::cout << "\n a.xgmin = " << a.xgmin(i) << " \n ";
                // std::cout << "\n a.xgmax = " << a.xgmax(i) << " \n ";
                // std::cout << "\n a.Nxg = " << a.Nxg(i) << " \n ";
                _xg.push_back( CAxis<T>(a.xgmin(i), a.xgmax(i), a.Nxg(i)) );
            }
            // std::cout << "\n 14 \n ";
            for (size_t i(0); i < a.xdim(); ++i) {
                _dx.push_back( (a.xgmax(i)-a.xgmin(i))/a.Nxg(i) );
            }
            // for (size_t i(0); i < a.pdim(); ++i) {
            //     _p.push_back( Axis<T>(a.pmax(i)/static_cast<T>(2 * a.Np(i)),a.pmax(i),a.Np(i)) );
            // }
            // std::cout << "\n 15 \n ";
            for (size_t i(0); i < a.pdim(); ++i) {
                // _p.push_back( CAxis<T>(a.pmin(i),a.pmax(i),a.Np(i)) );
                _p.push_back( CAxis<T>(a.pmin(i),a.dp(i)) );
            }
            for (size_t i(0); i < a.pdim(); ++i) {
                // _dp.push_back( a.pmax(i)/a.Np(i) );
                _dp.push_back( a.dp(i) );
            }

            for (size_t i(0); i < a.pxdim(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpx.push_back(a.dpx(i));
            }

            for (size_t i(0); i < a.pydim(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpy.push_back(a.dpy(i));
            }

            for (size_t i(0); i < a.pzdim(); ++i) {
                // _dp.push_back((_pmax[i])/(static_cast<T>(_Np[i])));
                // Non-uniform grid
                _dpz.push_back(a.dpz(i));
            }

            for (size_t i(0); i < a.pxdim(); ++i) {
                // _px.push_back( Axis<T>( static_cast<T>(-1.0) * a.pmax(i), a.pmax(i), a.Npx(i)) );
                _px.push_back( CAxis<T>( static_cast<T>(-1.0) * a.pmax(i), a.dpx(i)) );
            }
            for (size_t i(0); i < a.pydim(); ++i) {
                _py.push_back( CAxis<T>( static_cast<T>(-1.0) * a.pmax(i), a.dpy(i)) );
            }
            for (size_t i(0); i < a.pzdim(); ++i) {
                _pz.push_back( CAxis<T>( static_cast<T>(-1.0) * a.pmax(i), a.dpz(i)) );
            }
        }

        ~AxisBundle(){ }

    private:
        // vector< Axis<T> > _x, _xg, _p, _px;
        // 
        vector< CAxis<T> >  _px, _py, _pz;
        vector< CAxis<T> > _x, _xg, _p;
        vector<T>           _dx;//, _dp;
        // Non-uniform grid
        vector<valarray<T> > _dp;
        vector<valarray<T> > _dpx,_dpy,_dpz;
        // vector<valarray<T>> _dx, _dp;
    };
//--------------------------------------------------------------
#pragma optimize("", on)
//--------------------------------------------------------------
// LEGENDRE POLYNOMIALS
// Calculate the legendre polynomials using the recurrance relations
// For m0 = 0
    template<class T>
    valarray<T> Legendre(const T x, const size_t Nl){ // where Nl is to denote l0+1

        valarray<T> P_Legendre(0.0, Nl);
        if ( abs(x) > 1 ) return P_Legendre;

        T r1, sqrtx = sqrt(1.0-x*x);

//      Initialization 
        P_Legendre[0] = 1.0;
        P_Legendre[1] = x;

        for (size_t l(1); l < Nl - 1; ++l){
            r1 = 1.0 / double(l + 1);
            P_Legendre[l+1] = P_Legendre[l] * (x*(2.0*l+1.0) * r1) -
                              P_Legendre[l-1]*(double(l) * r1);
        }

        return P_Legendre;
    }

// For m0 > 0
    template<class T>
    Array2D<T> Legendre(const T x, const size_t Nl, const size_t Nm){

//      Local variables
        if (Nl < Nm) {
            cout << "ERROR: " << "l0+1 = " << Nl << " < " << Nm << " = m0+1\n";
            exit(1);
        }

        Array2D<T> P_Legendre(Nl+1,Nm+1);

        if ( abs(x) > 1 ) {
//             cout << "ERROR: " << "|" << x << "| > 1 is not a valid cosine\n";
//      cout << "setting values to 0.0";
            P_Legendre = 0.0;

        }
        else
        {

            T r1, sqrtx = sqrt(1.0-x*x), fact = 1.0;

            //      Initialization
            P_Legendre = 0.0;
            P_Legendre(0,0) = 1.0;

            for (size_t l = 1; l < Nm+1; ++l){
                // std::cout << "\n l=" << l;
                P_Legendre(l,l) = - P_Legendre(l-1,l-1)*(fact*sqrtx);
                fact += 2.0;
            }
            // std::cout << "\n ";

            for (size_t l = 0; l < ((Nm < Nl) ? Nm+1 : Nl); ++l){
                // std::cout << "\n l=" << l;
                P_Legendre(l+1,l) = P_Legendre(l,l)*(x*(2.0*l+1.0));
            }
            // std::cout << "\n ";

            for (size_t m = 0; m < Nm+1; ++m){
                for (size_t l = m+1; l < Nl ; ++l){
                    // std::cout << "\n l=" << l << ", m=" << m;
                    r1 = 1.0 / double(l - m + 1);
                    P_Legendre(l+1,m) = P_Legendre(l,m) * (x*(2.0*l+1.0) * r1) -
                                        P_Legendre(l-1,m)*(double(l+m) * r1);
                }
            }
        }

        return P_Legendre;
    }
//--------------------------------------------------------------
// MOMENTS
// p-th moment of a quantity q(x)
    template<class T>
    T moment(const vector<T> q, const vector<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      // integral += q[0] * (pow(x[0], p))                   // += Q_0*x_0^p * (x_1-x_0)
      //             * (x[1]-x[0]);
      // for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
      //     integral += q[i] * pow(x[i], p)
      //                 * (x[i+1] - x[i-1]);
      // }

        for (size_t i(1); i < q.size(); ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
            integral += (q[i]*pow(x[i],p)+q[i-1]*pow(x[i-1],p))*(x[i]-x[i-1]);
        }

      // integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
      //                 * (x[q.size()-1]-x[q.size()-2]);
      return integral*0.5;
    }
//--------------------------------------------------------------
//--------------------------------------------------------------
    template<class T>
    T moment(const vector<T> q, const valarray<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      // integral += q[0] * (pow(x[0], p))                   // += Q_0*x_0^p * (x_1-x_0)
      //             * (x[1]-x[0]);
      // for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
      //     integral += q[i] * pow(x[i], p)
      //                 * (x[i+1] - x[i-1]);
      // }
      // integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
      //                 * (x[q.size()-1]-x[q.size()-2]);
//                 
        for (size_t i(1); i < q.size(); ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
            integral += (q[i]*pow(x[i],p)+q[i-1]*pow(x[i-1],p))*(x[i]-x[i-1]);
        }
      return integral*0.5;
    }
//--------------------------------------------------------------
//--------------------------------------------------------------
    template<class T>
    T moment(const valarray<T> q, const valarray<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      // integral += q[0] * (pow(x[0], p))                   // += Q_0*x_0^p * (x_1-x_0)
      //             * (x[1]-x[0]);
      // for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
      //     integral += q[i] * pow(x[i], p)
      //                 * (x[i+1] - x[i-1]);
      // }
      // integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
      //                 * (x[q.size()-1]-x[q.size()-2]);
//                 
        for (size_t i(1); i < q.size(); ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
            integral += (q[i]*pow(x[i],p)+q[i-1]*pow(x[i-1],p))*(x[i]-x[i-1]);
        }
      return integral*0.5;

    }
//--------------------------------------------------------------
//--------------------------------------------------------------
    // template<class T>
    // T moment(const vector<T> q, const valarray<T> x, const valarray<T> dx, const int p){

    //     T integral(0.0);
    //     integral = 5./12. * q[0] * pow(x[0],p) * dx[0];
    //     integral += 13./12. * q[1] * pow(x[1],p) * dx[1];

    //     for (size_t i(2); i < q.size()-2; ++i)
    //     {  
    //         integral += q[i] * pow(x[i],p) * dx[i];
    //     }

    //     integral += 13./12. * q[q.size()-2] * pow(x[q.size()-2],p) * dx[q.size()-2];
    //     integral += 5./12. * q[q.size()-1] * pow(x[q.size()-1],p) * dx[q.size()-1];      
    // }

//--------------------------------------------------------------
// relativistic moments for inverse gamma and gamma
//--------------------------------------------------------------
    template<class T>
    T relativistic_invg_moment(const vector<T> q, const valarray<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      integral += q[0] * pow(x[0], p)                   // += Q_0*x_0^p * (x_1-x_0)
                  * (x[1]-x[0])
                  / sqrt(1.0+x[0]*x[0]);
      for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
          integral += q[i] * pow(x[i], p)
                      * (x[i+1] - x[i-1])
                      / sqrt(1.0+x[i]*x[i]);
      }
      integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
                      * (x[q.size()-1]-x[q.size()-2])
                      / sqrt(1.0+x[q.size()-1]*x[q.size()-1]);
      return integral*0.5;
    }
//--------------------------------------------------------------
//--------------------------------------------------------------    
    template<class T>
    T relativistic_invg_moment(const valarray<T> q, const valarray<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      integral += q[0] * pow(x[0], p)                   // += Q_0*x_0^p * (x_1-x_0)
                  * (x[1]-x[0])
                  / sqrt(1.0+x[0]*x[0]);
      for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
          integral += q[i] * pow(x[i], p)
                      * (x[i+1] - x[i-1])
                      / sqrt(1.0+x[i]*x[i]);
      }
      integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
                      * (x[q.size()-1]-x[q.size()-2])
                      / sqrt(1.0+x[q.size-1]*x[q.size-1]);
      return integral*0.5;
    }
//--------------------------------------------------------------
//--------------------------------------------------------------
    template<class T>
    T relativistic_gamma_moment(const valarray<T> q, const valarray<T> x, const int p){

        T integral(0.0);
// TODO:   Integral values up to the zeroth cell and above the last cell
      integral += q[0] * pow(x[0], p)                   // += Q_0*x_0^p * (x_1-x_0)
                  * (x[1]-x[0])
                  * sqrt(1.0+x[0]*x[0]);
      for (size_t i(1); i < q.size()-1; ++i){           // += Q_i*x_i^p * (x_{i+1}-x_{i-1})
          integral += q[i] * pow(x[i], p)
                      * (x[i+1] - x[i-1])
                      * sqrt(1.0+x[i]*x[i]);
      }
      integral += q[q.size()-1] * pow(x[q.size()-1], p) // += Q_n*x_n^p * (x_{n}-x_{n-1})
                      * (x[q.size()-1]-x[q.size()-2])
                      * sqrt(1.0+x[q.size-1]*x[q.size-1]);
      return integral*0.5;
    }
//--------------------------------------------------------------


//--------------------------------------------------------------
//  RUNGE-KUTTA METHODS
//--------------------------------------------------------------
    template<typename T> class AbstFunctor {
//  abstract functor interface 
    public:
        virtual void operator()(const T& Yin, T& Yslope)=0;  // call using operator
        virtual void operator()(const T& Yin, T& Yslope, double time, double dt)=0;  // call using operator
        virtual void operator()(const T& Yin, T& Yslope, size_t dir)=0;  // call using operator
    };


//  RK2 
    template<class T> class RK2 {
    public:
//      Constructor
        RK2(T& Yin): Y0(Yin), Yh(Yin) { }

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F);
        T& operator()(T& Y, double h, AbstFunctor<T>* F, size_t dir);

    private:
//      R-K copies for the data
        T  Y0, Yh;
    };

    template<class T> T& RK2<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F) {
//      Take a step using RK2

//      Initialization
        Y0 = Y;

//      Step 1
        (*F)(Y0,Yh); Yh *= h;     // Yh = h*F(Y0)      Yh += Y;  Yh = (*CF)(Yh,time,h);  Yh -= Y;
        Y0 += Yh;                 // Y0 = Y0 + h*Yh
        Yh *= 0.5; Y  += Yh;      // Y  = Y  + (h/2)*F(Y0)      
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y0,Yh); Yh *= 0.5*h; // Yh = (h/2)*F(Y0)
        Y += Yh;                  // Y = Y + (h/2)*F(Y0)
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
    template<class T> T& RK2<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F, size_t dir) {
//      Take a step using RK2

//      Initialization
        Y0 = Y;

//      Step 1
        (*F)(Y0,Yh,dir); Yh *= h;     // Yh = h*F(Y0)
        Y0 += Yh;                 // Y0 = Y0 + h*Yh
        Yh *= 0.5; Y  += Yh;      // Y  = Y  + (h/2)*F(Y0)      
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y0,Yh,dir); Yh *= 0.5*h; // Yh = (h/2)*F(Y0)
        Y += Yh;                  // Y = Y + (h/2)*F(Y0)
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//  RK3 (Osher&Shu Method) 
    template<class T> class RK3 {
    public:
//      Constructor
        RK3(T& Yin): Y0(Yin), Yh(Yin) { }

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F);
        T& operator()(T& Y, double h, AbstFunctor<T>* F, size_t dir);


    private:
//      R-K copies for the data
        T  Y0, Yh;
    };

    template<class T> T& RK3<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F) {
//      Take a step using RK3

//      Initialization
        Y0 = Y;

//      Step 1
        (*F)(Y0,Yh); Yh *= h;                         // Yh = h*F(Y0)
        Y0 += Yh;
//      Y0 = Y0 + h*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y0,Yh); Yh *= h;                      // Yh = h*F(Y0)
        Y0 += Yh; Y *= 3.0; Y0 += Y;  Y0 *= 0.25;  // Changed Y to 3*Y!
//      Y0 = 1/4 * ( 3*Y + (Y0 + h*Yh) )
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 3
        (*F)(Y0,Yh); Yh *= h;                        // Yh = h*F(Y0)
        Y0 += Yh; Y0 *= 6.0; Y += Y0; Y *= (1.0/9.0);
//      Y  = 1/3 * ( Y + 2 * (Y0+h*Yh) )
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

        return Y;
    }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    template<class T> T& RK3<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F, size_t dir) {
//      Take a step using RK3

//      Initialization
        Y0 = Y;

//      Step 1
        (*F)(Y0,Yh,dir); Yh *= h;                         // Yh = h*F(Y0)
        Y0 += Yh;
//      Y0 = Y0 + h*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y0,Yh,dir); Yh *= h;                      // Yh = h*F(Y0)
        Y0 += Yh; Y *= 3.0; Y0 += Y;  Y0 *= 0.25;  // Changed Y to 3*Y!
//      Y0 = 1/4 * ( 3*Y + (Y0 + h*Yh) )
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 3
        (*F)(Y0,Yh,dir); Yh *= h;                        // Yh = h*F(Y0)
        Y0 += Yh; Y0 *= 6.0; Y += Y0; Y *= (1.0/9.0);
//      Y  = 1/3 * ( Y + 2 * (Y0+h*Yh) )
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

        return Y;
    }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


//  RK4 
    template<class T> class RK4 {
    public:
//      Constructor
        RK4(T& Yin): Y0(Yin), Y1(Yin), Y2(Yin), Yh(Yin) { }

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F);
        T& operator()(T& Y, double h, AbstFunctor<T>* F, size_t dir);

    private:
//      R-K copies for the data
        T  Y0, Y1, Y2, Yh;
    };

    template<class T> T& RK4<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F) {
//      Take a step using RK4

//      Initialization
        Y0 = Y; Y1 = Y;

//      Step 1
        (*F)(Y1,Yh);                    // slope in the beginning
        Yh *= (0.5*h);   Y1 += Yh;      // Y1 = Y1 + (h/2)*Yh  Yhc = (*CF)(Y1,time,0.5*h)
        Yh *= (1.0/3.0); Y  += Yh;      // Y  = Y  + (h/6)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y1,Yh);     Y1  = Y0;      // slope in the middle
        Yh *= (0.5*h);   Y1 += Yh;      // Y1 = Y0 + (h/2)*Yh
        Yh *= (2.0/3.0); Y  += Yh;      // Y  = Y  + (h/3)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 3
        (*F)(Y1,Yh);                    // slope in the middle again
        Yh *= h;          Y0 += Yh;     // Y0 = Y0 + h*Yh
        Yh *= (1.0/3.0);  Y  += Yh;     // Y  = Y  + (h/3)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 4
        (*F)(Y0,Yh);                    // slope at the end
        Yh *= (h/6.0);    Y += Yh;      // Y  = Y  + (h/6)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

        return Y;
    }
    template<class T> T& RK4<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F, size_t dir) {
//      Take a step using RK4

//      Initialization
        Y0 = Y; Y1 = Y;

//      Step 1
        (*F)(Y1,Yh,dir);                    // slope in the beginning
        Yh *= (0.5*h);   Y1 += Yh;      // Y1 = Y1 + (h/2)*Yh
        Yh *= (1.0/3.0); Y  += Yh;      // Y  = Y  + (h/6)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 2
        (*F)(Y1,Yh,dir);     Y1  = Y0;      // slope in the middle
        Yh *= (0.5*h);   Y1 += Yh;      // Y1 = Y0 + (h/2)*Yh
        Yh *= (2.0/3.0); Y  += Yh;      // Y  = Y  + (h/3)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 3
        (*F)(Y1,Yh,dir);                    // slope in the middle again
        Yh *= h;          Y0 += Yh;     // Y0 = Y0 + h*Yh
        Yh *= (1.0/3.0);  Y  += Yh;     // Y  = Y  + (h/3)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//      Step 4
        (*F)(Y0,Yh,dir);                    // slope at the end
        Yh *= (h/6.0);    Y += Yh;      // Y  = Y  + (h/6)*Yh
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

        return Y;
    }

//  RKCK
    template<class T> class RKCK54 {
    public:
//      Constructor
        RKCK54(T& Yin): Yh1(Yin), Yh3(Yin), Yh4(Yin), Yh5(Yin), Yh6(Yin), Yt(Yin),
        a21(0.2), 
        a31(3./40.), a32(9./40.),
        a41(.3), a42(-.9), a43(1.2),
        a51(-11./54.), a52(2.5) ,a53(-70./27.), a54(35./27.),
        a61(1631./55296.), a62(175./512.), a63(575./13824.), a64(44275./110592.), a65(253./4096.),
        b1_5(37./378.), b3_5(250./621.), b4_5(125./594.), b6_5(512./1771.),
        b1_4(2825./27648.), b3_4(18575./48384.), b4_4(13525./55296.), b5_4(277./14336.), b6_4(0.25)
        {}

//      Main function
        T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F);
        // T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F, size_t dir);

    private:
//      R-K copies for the data
        T  Yh1, Yh3, Yh4, Yh5, Yh6, Yt;

        double a21;
        double a31,a32;
        double a41,a42,a43;
        double a51,a52,a53,a54;
        double a61,a62,a63,a64,a65;

        double b1_5,b3_5,b4_5,b6_5;
        double b1_4,b3_4,b4_4,b5_4,b6_4;
    };

    template<class T> T& RKCK54<T>::operator()
            (T& Y5, T& Y4, double h, AbstFunctor<T>* F) {
//      Take a step using RKCK54

//      Initialization
        // Yh1 = Y5;   Yh3 = Y5;   Yh4 = Y5;   Yh5 = Y5;   Yh6 = Y5;
        // Yt = Y5;

//      Step 1
        (*F)(Y4,Yh1); Yh1 *= h;
        Yh1 *= a21;
        Yt = Y4;    Yt  += Yh1;                              // Y1 = Y1 + (h/5)*Yh

        //      Step 2
        // (*F)(Yt,Yh2);                                   // f(Y1)
        (*F)(Yt,Y5); Y5 *= h;                                   // f(Y1)
        Yh1 *= a31/a21; Y5 *= a32;  
        Yt = Y4;    Yt += Yh1;  Yt += Y5;

        //      Step 3
        (*F)(Yt,Yh3); Yh3 *= h;
        Yh1 *= a41/a31; Y5 *= a42/a32;   Yh3 *= a43;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3;
        
        //      Step 4
        (*F)(Yt,Yh4); Yh4 *= h;
        Yh1 *= a51/a41; Y5 *= a52/a42;   Yh3 *= a53/a43;    Yh4 *= a54;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3; Yt += Yh4;
        
        //      Step 5
        (*F)(Yt,Yh5); Yh5 *= h;
        Yh1 *= a61/a51; Y5 *= a62/a52;   Yh3 *= a63/a53;    Yh4 *= a64/a54; Yh5 *= a65;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3; Yt += Yh4; Yt += Yh5;
            

        //      Step 6
        (*F)(Yt,Yh6); Yh6 *= h;


        //      Assemble 5th order solution
        Y5 = Y4;
        Yh1 *= b1_5/a61;    Y5 += Yh1;
        Yh3 *= b3_5/a63;    Y5 += Yh3;
        Yh4 *= b4_5/a64;    Y5 += Yh4;
        Yh6 *= b6_5;        Y5 += Yh6;

        //      Assemble 4th order solution
        Yh1 *= b1_4/b1_5;   Y4 += Yh1;
        Yh3 *= b3_4/b3_5;   Y4 += Yh3;
        Yh4 *= b4_4/b4_5;   Y4 += Yh4;
        Yh5 *= b5_4/a65;    Y4 += Yh5;
        Yh6 *= b6_4/b6_5;   Y4 += Yh6;

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // return Y;
    }

//--------------------------------------------------------------
//  RKCK
    template<class T> class RKBS54 {
    public:
//      Constructor
        RKBS54(T& Yin): Yh1(Yin), Yh3(Yin), Yh4(Yin), Yh5(Yin), Yh6(Yin), Yh7(Yin), Yt(Yin),
        a21(1.0/6.0), 
        a31(2./27.), a32(4./27.),
        a41(183./1372.), a42(-162./343.), a43(1053./1372.),
        a51(68./297.), a52(-4./11.), a53(42./143.), a54(1960./3861.),
        a61(597./22528.), a62(81./352.), a63(63099./585728.), a64(58653./366080.), a65(4617./20480.),
        a71(174197./959244.), a72(-30942./79937.), a73(8152137./19744439.), a74(666106./1039181.), a75(-29421./29068.), a76(482048./414219.),
        a81(587./8064.), a83(4440339./15491840.), a84(24353./124800.), a85(387./44800.), a86(2152./5985.), a87(7267./94080.),
        b1_5(587./8064.), b3_5(4440339./15491840.), b4_5(24353./124800.), b5_5(387./44800.), b6_5(2152./5985.), b7_5(7267./94080.),
        bw1_4(6059./80640.), bw3_4(8559189./30983680.), bw4_4(26411./124800.), bw5_4(-927./89600.), bw6_4(443./1197.), bw7_4(7267./94080.),
        
        bs1_4(2479./34992.), bs3_4(123./416.), bs4_4(612941./3411720.), bs5_4(43./1440.), bs6_4(2272./6651.), bs7_4(79937./1113912.), bs8_4(3293./556956.)
        {}

//      Main function
        T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F);
        // T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F, size_t dir);

    private:
//      R-K copies for the data
        T  Yh1, Yh3, Yh4, Yh5, Yh6, Yh7, Yt;

        double a21;
        double a31,a32;
        double a41,a42,a43;
        double a51,a52,a53,a54;
        double a61,a62,a63,a64,a65;
        double a71,a72,a73,a74,a75,a76;
        double a81,a82,a83,a84,a85,a86,a87;

        double b1_5,b3_5,b4_5,b5_5,b6_5,b7_5;
        double bw1_4,bw3_4,bw4_4,bw5_4,bw6_4, bw7_4;
        double bs1_4,bs3_4,bs4_4,bs5_4,bs6_4, bs7_4, bs8_4;
    };

    template<class T> T& RKBS54<T>::operator()
            (T& Y5, T& Y4, double h, AbstFunctor<T>* F) {
//      Take a step using RKBS54

//      Initialization
        // Yh1 = Y5;   Yh3 = Y5;   Yh4 = Y5;   Yh5 = Y5;   Yh6 = Y5;
        // Yt = Y5;

//      Step 1
        (*F)(Y4,Yh1); Yh1 *= h;
        Yh1 *= a21;
        Yt = Y4;    Yt  += Yh1;                              // Y1 = Y1 + (h/5)*Yh

        //      Step 2
        // (*F)(Yt,Yh2);                                   // f(Y1)
        (*F)(Yt,Y5); Y5 *= h;                                   // f(Y1)
        Yh1 *= a31/a21; Y5 *= a32;  
        Yt = Y4;    Yt += Yh1;  Yt += Y5;

        //      Step 3
        (*F)(Yt,Yh3); Yh3 *= h;
        Yh1 *= a41/a31; Y5 *= a42/a32;   Yh3 *= a43;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3;
        
        //      Step 4
        (*F)(Yt,Yh4); Yh4 *= h;
        Yh1 *= a51/a41; Y5 *= a52/a42;   Yh3 *= a53/a43;    Yh4 *= a54;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3; Yt += Yh4;
        
        //      Step 5
        (*F)(Yt,Yh5); Yh5 *= h;
        Yh1 *= a61/a51; Y5 *= a62/a52;   Yh3 *= a63/a53;    Yh4 *= a64/a54; Yh5 *= a65;
        Yt = Y4;    Yt += Yh1; Yt += Y5; Yt += Yh3; Yt += Yh4; Yt += Yh5;
            
        //      Step 6
        (*F)(Yt,Yh6); Yh6 *= h;
        Yh1 *= a71/a61; Y5 *= a72/a62;   Yh3 *= a73/a63;    Yh4 *= a74/a64; Yh5 *= a75/a65; Yh6 *= a76;
        Yt = Y4;    Yt += Yh1;  Yt += Y5;   Yt += Yh3;  Yt += Yh4;  Yt += Yh5;  Yt += Yh6;

        //      Step 7
        (*F)(Yt,Yh7); Yh7 *= h;


        //      Assemble 5th order solution
        Y5 = Y4;
        Yh1 *= b1_5/a71;    Y5 += Yh1;
        Yh3 *= b3_5/a73;    Y5 += Yh3;
        Yh4 *= b4_5/a74;    Y5 += Yh4;
        Yh5 *= b5_5/a75;    Y5 += Yh5;
        Yh6 *= b6_5/a76;    Y5 += Yh6;
        Yh7 *= b7_5;        Y5 += Yh7;

        //      Assemble 4th order solution
        Yh1 *= bw1_4/b1_5;  Y4 += Yh1;
        Yh3 *= bw3_4/b3_5;  Y4 += Yh3;
        Yh4 *= bw4_4/b4_5;  Y4 += Yh4;
        Yh5 *= bw5_4/b5_5;  Y4 += Yh5;
        Yh6 *= bw6_4/b6_5;  Y4 += Yh6;
        Yh7 *= bw7_4/b7_5;  Y4 += Yh7;

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // return Y;
    }
//--------------------------------------------------------------
//  RKTsitouras
    template<class T> class RKT54 {
    public:
//      Constructor
        RKT54(T& Yin): Yh1(Yin), Yh2(Yin), Yh3(Yin), Yh4(Yin), Yh5(Yin), Yh6(Yin), Yh7(Yin), Yt(Yin),
        
        a21(0.161),
        a31(-0.008480655492356989),   a32(0.335480655492357),
        a41(2.8971530571054935),    a42(-6.359448489975075),    a43(4.3622954328695815),
        a51(5.325864828439257),   a52(-11.748883564062828),  a53(7.4955393428898365),  a54(-0.09249506636175525),
        a61(5.86145544294642),    a62(-12.92096931784711),    a63(8.159367898576159), a64(-0.071584973281401), a65(-0.028269050394068383),
        a71(0.09646076681806523), a72(0.01),   a73(0.4798896504144996),   a74(1.379008574103742),    a75(-3.290069515436081),    a76(2.324710524099774),

        b1_5(0.09468075576583945),    b2_5(0.009183565540343254),   b3_5(0.4877705284247616),    b4_5(1.234297566930479),  b5_5(-2.7077123499835256),  b6_5(1.866628418170587),    b7_5(0.015151515151515152),
        btilde1(-0.00178001105222577714),   btilde2(-0.0008164344596567469),   btilde3(0.007880878010261995), btilde4(-0.1447110071732629),    btilde5(0.5823571654525552),    btilde6(-0.45808210592918697),  btilde7(0.015151515151515152)
        {}

//      Main function
        T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F);
        // T& operator()(T& Y5, T&Y4, double h, AbstFunctor<T>* F, size_t dir);

    private:
//      R-K copies for the data
        T  Yh1, Yh2, Yh3, Yh4, Yh5, Yh6, Yh7, Yt;

        double a21;
        double a31,a32;
        double a41,a42,a43;
        double a51,a52,a53,a54;
        double a61,a62,a63,a64,a65;
        double a71,a72,a73,a74,a75,a76;
        

        double b1_5,b2_5,b3_5,b4_5,b5_5,b6_5, b7_5;
        double btilde1, btilde2, btilde3, btilde4, btilde5, btilde6, btilde7;
    };

    template<class T> T& RKT54<T>::operator()
            (T& Y5, T& Y4, double h, AbstFunctor<T>* F) {
//      Take a step using RKT54

//      Initialization
        // Yh1 = Y5;   Yh3 = Y5;   Yh4 = Y5;   Yh5 = Y5;   Yh6 = Y5;
        // Yt = Y5;

//      Step 1
        (*F)(Y4,Yh1); Yh1 *= h;
        Yh1 *= a21;
        Yt = Y4;    Yt  += Yh1;                              // Y1 = Y1 + (h/5)*Yh

        //      Step 2
        // (*F)(Yt,Yh2);                                   // f(Y1)
        (*F)(Yt,Yh2); Yh2 *= h;                                   // f(Y1)
        Yh1 *= a31/a21; Yh2 *= a32;  
        Yt = Y4;    Yt += Yh1;  Yt += Yh2;

        //      Step 3
        (*F)(Yt,Yh3); Yh3 *= h;
        Yh1 *= a41/a31; Yh2 *= a42/a32;   Yh3 *= a43;
        Yt = Y4;    Yt += Yh1; Yt += Yh2; Yt += Yh3;
        
        //      Step 4
        (*F)(Yt,Yh4); Yh4 *= h;
        Yh1 *= a51/a41; Yh2 *= a52/a42;   Yh3 *= a53/a43;    Yh4 *= a54;
        Yt = Y4;    Yt += Yh1; Yt += Yh2; Yt += Yh3; Yt += Yh4;
        
        //      Step 5
        (*F)(Yt,Yh5); Yh5 *= h;
        Yh1 *= a61/a51; Yh2 *= a62/a52;   Yh3 *= a63/a53;    Yh4 *= a64/a54; Yh5 *= a65;
        Yt = Y4;    Yt += Yh1; Yt += Yh2; Yt += Yh3; Yt += Yh4; Yt += Yh5;
            
        //      Step 6
        (*F)(Yt,Yh6); Yh6 *= h;
        Yh1 *= a71/a61; Yh2 *= a72/a62;   Yh3 *= a73/a63;    Yh4 *= a74/a64; Yh5 *= a75/a65; Yh6 *= a76;
        Yt = Y4;    Yt += Yh1;  Yt += Yh2;   Yt += Yh3;  Yt += Yh4;  Yt += Yh5;  Yt += Yh6;

        //      Step 7
        (*F)(Yt,Yh7); Yh7 *= h;

        //      Assemble 5th order solution
        Y5 = Y4;
        Yh1 *= b1_5/a71;    Y5 += Yh1;
        Yh2 *= b2_5/a72;    Y5 += Yh2;
        Yh3 *= b3_5/a73;    Y5 += Yh3;
        Yh4 *= b4_5/a74;    Y5 += Yh4;
        Yh5 *= b5_5/a75;    Y5 += Yh5;
        Yh6 *= b6_5/a76;    Y5 += Yh6;
        Yh7 *= b7_5;        Y5 += Yh7;

        //      Assemble 4th order solution
        Yh1 *= btilde1/b1_5;  Y4 += Yh1;
        Yh2 *= btilde2/b2_5;  Y4 += Yh3;
        Yh3 *= btilde3/b3_5;  Y4 += Yh3;
        Yh4 *= btilde4/b4_5;  Y4 += Yh4;
        Yh5 *= btilde5/b5_5;  Y4 += Yh5;
        Yh6 *= btilde6/b6_5;  Y4 += Yh6;
        Yh7 *= btilde7/b7_5;  Y4 += Yh7;

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // return Y;
    }

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------    
//  ARK
    template<class T> class ARK32 {
    public:
//      Constructor
        ARK32(T& Yin): Yhv1(Yin), Yhv2(Yin), Yhv3(Yin), Yhv4(Yin),
                        Yhc2(Yin), Yhc3(Yin), Yhc4(Yin), 
                        Yt(Yin),
        ae21(1767732205903./2027836641118.),
        ae31(5535828885825./10492691773637.), ae32(788022342437./10882634858940.),
        ae41(6485989280629./16251701735622.), ae42(-4246266847089./9704473918619), ae43(10755448449292./10357097424841),
        ai21(1767732205903./4055673282236), ai22(1767732205903./4055673282236.),
        ai31(2746238789719./10658868560708.), ai32(-640167445237./6845629431997), ai33(1767732205903./4055673282236.),
        ai41(1471266399579./7840856788654.), ai42(-4482444167858./7529755066697), ai43(11266239266428./11593286722821.), ai44(1767732205903./4055673282236.),

        b1(1471266399579./7840856788654.), b2(-4482444167858./7529755066697), b3(11266239266428./11593286722821.), b4(1767732205903./4055673282236.),
        b1_LO(2756255671327./12835298489170.), b2_LO(-10771552573575./22201958757719), b3_LO(9247589265047./10645013368117), b4_LO(2193209047091./5459859503100)
        {}

//      Main function
        T& operator()(T& Y3, T&Y2, T& Yhc1, double time, double h, AbstFunctor<T>* vF,  AbstFunctor<T>* cF);

    private:
//      R-K copies for the data
        T  Yhv1, Yhv2, Yhv3, Yhv4, Yhc2, Yhc3, Yhc4, Yt;

        double ae21;
        double ae31,ae32;
        double ae41,ae42,ae43;
        double ai21, ai22;
        double ai31,ai32,ai33;
        double ai41,ai42,ai43,ai44;
    
        double b1,b2,b3,b4;
        double b1_LO,b2_LO,b3_LO,b4_LO;
    };

    template<class T> T& ARK32<T>::operator()
            (T& Y3, T& Y2, T& Yhc1, double time, double h, AbstFunctor<T>* vF, AbstFunctor<T>* cF) {
//      Take a step using ARK32

//      Yh1, Stage 1
        // z1 = Y2;

        (*vF)(Y2,Yhv1);
        Yhv1 *= ae21*h; Yhc1 *= ai21*h;
        
        Yt = Y2;
        Yt += Yhv1; Yt += Yhc1;     
        
        (*cF)(Yt,Yhc2,time,(ai22*h));
        Yhc2 *= (ai22*h);    Yt += Yhc2;
        
        // z2 = Yt;

        (*vF)(Yt,Yhv2);
        Yhv1 *= ae31/ae21;  Yhc1 *= ai31/ai21;  
        Yhv2 *= ae32*h;     Yhc2 *= ai32/ai22;  

        Yt = Y2;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        (*cF)(Yt,Yhc3,time,(ai33*h));

        Yhc3 *= (ai33*h);   Yt += Yhc3;
        
        // z3 = Yt;

        (*vF)(Yt,Yhv3);
        Yhv1 *= ae41/ae31;  Yhc1 *= ai41/ai31;  
        Yhv2 *= ae42/ae32;  Yhc2 *= ai42/ai32;  
        Yhv3 *= ae43*h;     Yhc3 *= ai43/ai33;

        Yt = Y2;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        Yt += Yhv3; Yt += Yhc3;

        (*cF)(Yt,Yhc4,time,(ai44*h));

        Yhc4 *= (ai44*h);   Yt += Yhc4;
        
        // z4 = Yt;

        (*vF)(Yt,Yhv4);

        //  Assemble 3rd order solution
        Y3 = Y2;
        Yhv1 *= b1/ae41;    Y3 += Yhv1;
        Yhv2 *= b2/ae42;    Y3 += Yhv2;
        Yhv3 *= b3/ae43;    Y3 += Yhv3;
        Yhv4 *= b4*h;       Y3 += Yhv4;

        Yhc1 *= b1/ai41;    Y3 += Yhc1;
        Yhc2 *= b2/ai42;    Y3 += Yhc2;
        Yhc3 *= b3/ai43;    Y3 += Yhc3;
        Yhc4 *= b4/ai44;    Y3 += Yhc4;

        //  Assemble 2nd order solution
        Yhv1 *= b1_LO/b1;   Y2 += Yhv1;
        Yhv2 *= b2_LO/b2;   Y2 += Yhv2;
        Yhv3 *= b3_LO/b3;   Y2 += Yhv3;
        Yhv4 *= b4_LO/b4;   Y2 += Yhv4;

        Yhc1 *= b1_LO/b1;   Y2 += Yhc1;
        Yhc2 *= b2_LO/b2;   Y2 += Yhc2;
        Yhc3 *= b3_LO/b3;   Y2 += Yhc3;
        Yhc4 *= b4_LO/b4;   Y2 += Yhc4;

        Yhc4 *= 1./(b4_LO*h);
        Yhc1 = Yhc4;

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // return Y;
    }
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------    
//  ARK
    template<class T> class ARK43 {
    public:
//      Constructor
        ARK43(T& Yin): Yhv1(Yin), Yhv2(Yin), Yhv3(Yin), Yhv4(Yin), Yhv5(Yin), Yhv6(Yin),
                        Yhc2(Yin), Yhc3(Yin), Yhc4(Yin), Yhc5(Yin), Yhc6(Yin), 
                        Yt(Yin),
        
        ae21(0.5),
        ae31(13861.0/62500.0), ae32(6889.0/62500.0),
        ae41(-116923316275.0/2393684061468.0), ae42(-2731218467317.0/15368042101831.0), ae43(9408046702089.0/11113171139209.0),
        ae51(-451086348788.0/2902428689909.0), ae52(-2682348792572.0/7519795681897.0), ae53(12662868775082.0/11960479115383.0), ae54(3355817975965.0/11060851509271.0),
        ae61(647845179188.0/3216320057751.0), ae62(73281519250.0/8382639484533.0), ae63(552539513391.0/3454668386233.0), ae64(3354512671639.0/8306763924573.0), ae65(4040.0/17871.0),
        
        ai21(1.0/4.0), ai22(1.0/4.0),
        ai31(8611.0/62500.0), ai32(-1743.0/31250.0), ai33(1.0/4.0),
        ai41(5012029.0/34652500.0), ai42(-654441.0/2922500.0), ai43(174375.0/388108.0), ai44(1.0/4.0),
        ai51(15267082809.0/155376265600.0), ai52(-71443401.0/120774400.0), ai53(730878875.0/902184768.0), ai54(2285395.0/8070912.0), ai55(1.0/4.0),
        ai61(82889.0/524892.0), ai63(15625.0/83664.0), ai64(69875.0/102672.0), ai65(-2260.0/8211.0), ai66(1.0/4.0),

        b1(82889.0/524892.0), b3(15625.0/83664.0), b4(69875.0/102672.0), b5(-2260.0/8211.0), b6(1.0/4.0),
        b1_LO(4586570599.0/29645900160.0), b3_LO(178811875.0/945068544.0), b4_LO(814220225.0/1159782912.0), b5_LO(-3700637.0/11593932.0), b6_LO(61727.0/225920.0)
        {}

//      Main function
        T& operator()(T& Y3, T&Y4, T& Yhc1, double time, double h, AbstFunctor<T>* vF,  AbstFunctor<T>* cF);

    private:
//      R-K copies for the data
        T  Yhv1, Yhv2, Yhv3, Yhv4, Yhv5, Yhv6;
        T  Yhc2, Yhc3, Yhc4, Yhc5, Yhc6, Yt;

        double ae21;
        double ae31,ae32;
        double ae41,ae42,ae43;
        double ae51,ae52,ae53,ae54;
        double ae61,ae62,ae63,ae64,ae65;
        
        double ai21, ai22;
        double ai31,ai32,ai33;
        double ai41,ai42,ai43,ai44;
        double ai51,ai52,ai53,ai54,ai55;
        double ai61,ai62,ai63,ai64,ai65,ai66;
    
        double b1,b2,b3,b4,b5,b6;
        double b1_LO,b3_LO,b4_LO,b5_LO,b6_LO;
    };

    template<class T> T& ARK43<T>::operator()
            (T& Y3, T& Y4, T& Yhc1, double time, double h, AbstFunctor<T>* vF, AbstFunctor<T>* cF) {
//      Take a step using ARK43

//      Yh1, Stage 1
        // z1 = Y2;

        (*vF)(Y4,Yhv1);// PE.Neighbor_Communications(Yhv1);
        Yhv1 *= ae21*h; Yhc1 *= ai21*h;
        
        Yt = Y4;
        Yt += Yhv1; Yt += Yhc1;     
        
        

        (*cF)(Yt,Yhc2,time,(ai22*h));   
        Yhc2 *= (ai22*h);    Yt += Yhc2;
        
        // z2 = Yt;

        (*vF)(Yt,Yhv2);// PE.Neighbor_Communications(Yhv2);
        Yhv1 *= ae31/ae21;  Yhc1 *= ai31/ai21;  
        Yhv2 *= ae32*h;     Yhc2 *= ai32/ai22;  

        Yt = Y4;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        (*cF)(Yt,Yhc3,time,(ai33*h));

        Yhc3 *= (ai33*h);   Yt += Yhc3;
        
        // z3 = Yt;

        (*vF)(Yt,Yhv3);// PE.Neighbor_Communications(Yhv3);
        Yhv1 *= ae41/ae31;  Yhc1 *= ai41/ai31;  
        Yhv2 *= ae42/ae32;  Yhc2 *= ai42/ai32;  
        Yhv3 *= ae43*h;     Yhc3 *= ai43/ai33;

        Yt = Y4;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        Yt += Yhv3; Yt += Yhc3;

        (*cF)(Yt,Yhc4,time,(ai44*h));

        Yhc4 *= (ai44*h);   Yt += Yhc4;
        
        // z4 = Yt;
        (*vF)(Yt,Yhv4);// PE.Neighbor_Communications(Yhv4);
        Yhv1 *= ae51/ae41;  Yhc1 *= ai51/ai41;  
        Yhv2 *= ae52/ae42;  Yhc2 *= ai52/ai42;  
        Yhv3 *= ae53/ae43;  Yhc3 *= ai53/ai43;  
        Yhv4 *= ae54*h;     Yhc4 *= ai54/ai44;

        Yt = Y4;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        Yt += Yhv3; Yt += Yhc3;
        Yt += Yhv4; Yt += Yhc4;

        (*cF)(Yt,Yhc5,time,(ai55*h));

        Yhc5 *= (ai55*h);   Yt += Yhc5;
        
        // z5 = Yt;
        (*vF)(Yt,Yhv4);
        Yhv1 *= ae61/ae51;  Yhc1 *= ai61/ai51;  
        Yhv2 *= ae62/ae52;  Yhc2 *= ai62/ai52;  
        Yhv3 *= ae63/ae53;  Yhc3 *= ai63/ai53;  
        Yhv4 *= ae64/ae54;  Yhc4 *= ai64/ai54;  
        Yhv5 *= ae65*h;     Yhc5 *= ai65/ai55;

        Yt = Y4;
        Yt += Yhv1; Yt += Yhc1;
        Yt += Yhv2; Yt += Yhc2;
        Yt += Yhv3; Yt += Yhc3;
        Yt += Yhv4; Yt += Yhc4;
        Yt += Yhv5; Yt += Yhc5;

        (*cF)(Yt,Yhc6,time,(ai66*h));

        Yhc5 *= (ai66*h);   Yt += Yhc6;

        // z6 = Yt;
        (*vF)(Yt,Yhv6);

        //  Assemble 3rd order solution
        Y3 = Y4;
        Yhv1 *= b1/ae61;    Y3 += Yhv1;
        Yhv3 *= b3/ae63;    Y3 += Yhv3;
        Yhv4 *= b4/ae64;    Y3 += Yhv4;
        Yhv5 *= b5/ae65;    Y3 += Yhv5;
        Yhv6 *= b6*h;       Y3 += Yhv6;

        Yhc1 *= b1/ai61;    Y3 += Yhc1;
        Yhc3 *= b3/ai63;    Y3 += Yhc3;
        Yhc4 *= b4/ai64;    Y3 += Yhc4;
        Yhc5 *= b5/ai65;    Y3 += Yhc5;
        Yhc6 *= b6/ai66;    Y3 += Yhc6;

        //  Assemble 2nd order solution
        Yhv1 *= b1_LO/b1;   Y4 += Yhv1;
        Yhv3 *= b3_LO/b3;   Y4 += Yhv3;
        Yhv4 *= b4_LO/b4;   Y4 += Yhv4;
        Yhv5 *= b5_LO/b5;   Y4 += Yhv5;
        Yhv6 *= b6_LO/b6;   Y4 += Yhv6;

        Yhc1 *= b1_LO/b1;   Y4 += Yhc1;
        Yhc3 *= b3_LO/b3;   Y4 += Yhc3;
        Yhc4 *= b4_LO/b4;   Y4 += Yhc4;
        Yhc5 *= b5_LO/b5;   Y4 += Yhc5;
        Yhc6 *= b6_LO/b6;   Y4 += Yhc6;

        Yhc4 *= 1./(b4_LO*h);
        Yhc1 = Yhc4;

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // return Y;
    }

//--------------------------------------------------------------
    //  Leapfrog space (Position verlet)
    template<class T> class LEAPs {
    public:
//      Constructor
        LEAPs(T& Yin): Y0(Yin), Yh(Yin) { }

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, AbstFunctor<T>* F_field);
        // T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir);

    private:
//      R-K copies for the data
        T  Y0, Yh;
    };

    template<class T> T& LEAPs<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum,
                AbstFunctor<T>* F_field) {
//      Take a step using LEAPspace

//      Initialization
        Y0 = Y;

        (*F_space)(Y0,Yh);      Yh *= 0.5*h;            //  x*  = h/2 * F_space(Y0)
        Y0  += Yh;
        (*F_field)(Y0,Yh);      Yh *= 0.5*h;            //  E*  = h/2 * F_field(Y0(x*))
        Y0  += Yh;
        (*F_momentum)(Y0,Yh);   Yh *= h;                //  p  = h * F_momentum(Y0(x*,E*))
        Y0  += Yh;                                   
        (*F_space)(Y0,Yh);      Yh *= 0.5*h;            //  x  = h/2 * F_space(Y0(p))
        Y0  += Yh;
        (*F_field)(Y0,Yh);      Yh *= 0.5*h;            //  E*  = h/2 * F_field(Y0(x*))
        Y0  += Yh;
        
        Y = Y0;
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
//     template<class T> T& LEAPs<T>::operator()
//             (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir) {
// //      Take a step using LEAPspace

// //      Initialization
//         Y0 = Y;

// //      First step momentum
//         (*F_space)(Y0,Yh);      Yh *= 0.5*h;            //  x  = h * F_space(Y0)
//         Y0  += Yh;
//         (*F_momentum)(Y0,Yh);   Yh *= h;            //  p  = h * F_momentum(Y0)
//         Y0 += Yh;                                   //  Y0 = Y0 + h*Yh
//         (*F_space)(Y0,Yh);      Yh *= 0.5*h;            //  x  = h * F_space(Y0)
//         Y0  += Yh;

//         Y = Y0;
// //      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//         return Y;
//     }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//--------------------------------------------------------------

    //  Leapfrog space (velocity verlet)
    template<class T> class LEAPv {
    public:
//      Constructor
        LEAPv(T& Yin): Y0(Yin), Yh(Yin) { }

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, AbstFunctor<T>* F_field);
        // T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir);

    private:
//      R-K copies for the data
        T  Y0, Yh;
    };

    template<class T> T& LEAPv<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum,
                AbstFunctor<T>* F_field) {
//      Take a step using LEAPmomentum

//      Initialization
        Y0 = Y;

        (*F_momentum)(Y0,Yh);      Yh *= 0.5*h;            //  x  = h * F_space(Y0)
        Y0  += Yh;
        (*F_space)(Y0,Yh);   Yh *= h;            //  p  = h * F_momentum(Y0)
        Y0 += Yh; 
        (*F_field)(Y0,Yh);      Yh *= h;            //  E*  = h/2 * F_field(Y0(x*))
        Y0  += Yh;
        (*F_momentum)(Y0,Yh);      Yh *= 0.5*h;            //  x  = h * F_space(Y0)
        Y0  += Yh;

        Y = Y0;
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
//     template<class T> T& LEAPv<T>::operator()
//             (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir) {
// //      Take a step using LEAPmomentum

// //      Initialization
//         Y0 = Y;

        
//         (*F_momentum)(Y0,Yh);      Yh *= 0.5*h;            //  p*  = h/2 * F_momentum(Y0)
//         Y0  += Yh;
        
//         (*F_space)(Y0,Yh);   Yh *= h;                   //  x  = h * F_space(Y0)
//         Y0 += Yh;

//         (*F_field)(Y0,Yh);      Yh *= 0.5*h;            //  E*  = h/2 * F_field(Y0(x*))
//         Y0  += Yh;                                   
//         (*F_momentum)(Y0,Yh);      Yh *= 0.5*h;         //  p  = h * F_space(Y0)
//         Y0  += Yh;
//         (*F_field)(Y0,Yh);      Yh *= 0.5*h;            //  E*  = h/2 * F_field(Y0(x*))
//         Y0  += Yh;

//         Y = Y0;
// //      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//         return Y;
//     }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    //  Position Extended Forest-Ruth Like
    template<class T> class PEFRL {
    public:
//      Constructor
        PEFRL(T& Yin): Y0(Yin), Yh(Yin),
                       xsi(0.1786178958448091),
                       lambda(-0.2123418310626054),
                       chi(-0.06626458266981849)
        {}

//      Main function
        T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, AbstFunctor<T>* F_field);
        T& operator()(T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir);

    private:
//      R-K copies for the data
        T  Y0, Yh;
        double xsi, lambda, chi;
    };

    template<class T> T& PEFRL<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum,  AbstFunctor<T>* F_field) {
//      Take a step using PEFRL

//      Initialization
        Y0 = Y;

//      First step space
        (*F_space)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        // (*F_field)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        // Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= (1.0-2.0*lambda)*0.5*h;            //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                   //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        // (*F_field)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        // Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= lambda*h;             //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                           //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= (1.0-2.0*(chi+xsi))*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        // (*F_field)(Y0,Yh);      Yh *= (1.0-2.0*(chi+xsi))*h;                              //  x  = h * F_space(Y0)
        // Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= lambda*h;             //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                           //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        // (*F_field)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        // Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= (1.0-2.0*lambda)*0.5*h;            //  p  = h * F_momentum(Y0)
        Y0 += Yh;

        (*F_space)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        // (*F_field)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        // Y0 += Yh;        

        (*F_field)(Y0,Yh);      Yh *= h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;        
        
        Y = Y0;
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
    template<class T> T& PEFRL<T>::operator()
            (T& Y, double h, AbstFunctor<T>* F_space, AbstFunctor<T>* F_momentum, size_t dir) {
//      Take a step using PEFRL

//      Initialization
        Y0 = Y;

//      First step space
        (*F_space)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= (1.0-2.0*lambda)*0.5*h;            //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                   //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= lambda*h;             //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                           //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= (1.0-2.0*(chi+xsi))*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= lambda*h;             //  p  = h * F_momentum(Y0)
        Y0 += Yh;                                           //  Y0 = Y0 + h*Yh

        (*F_space)(Y0,Yh);      Yh *= chi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;

        (*F_momentum)(Y0,Yh);   Yh *= (1.0-2.0*lambda)*0.5*h;            //  p  = h * F_momentum(Y0)
        Y0 += Yh;

        (*F_space)(Y0,Yh);      Yh *= xsi*h;                              //  x  = h * F_space(Y0)
        Y0 += Yh;        
        
        Y = Y0;
//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        return Y;
    }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        ///  IMEX BDF4
    template<class T> class IMEXBDF4 {
    public:
//      Constructor
        IMEXBDF4(T& Yin): Y(Yin), Yh(Yin),
        a0(48./25.), a1(-36./25.), a2(16./25.), a3(-3./25.),
        v0(48./25.), v1(-72./25.), v2(48./25.), v3(-12./25.),
        c0(12./25.) { }

//      Main function
        T& operator()(T& Ym3, T& Ym2, T& Ym1, T& Yn, double time, double h, AbstFunctor<T>* vF, AbstFunctor<T>* cF);

    private:
//      R-K copies for the data
        T  Y, Yh;
        double a0,a1,a2,a3;
        double v0,v1,v2,v3;
        double c0;
    };

    template<class T> T& IMEXBDF4<T>::operator()
            (T& Ym3, T& Ym2, T& Ym1, T& Yn, double time, double h, AbstFunctor<T>* vF, AbstFunctor<T>* cF){
//      Take a step using IMEXBDF
        Y  = Yn;

        Yh = Yn;    Yh *= (a0-1.);  Y += Yh;       // Yn+1 = a0*Yn
        Yh = Ym1;   Yh *= a1;       Y += Yh;
        Yh = Ym2;   Yh *= a2;       Y += Yh;
        Yh = Ym3;   Yh *= a3;       Y += Yh;
        

        (*vF)(Ym3,Yh);      Yh *= v3*h;            // df/dt at F_{n-3}
        Y += Yh;
        (*vF)(Ym2,Yh);      Yh *= v2*h;            // df/dt at F_{n-2}
        Y += Yh;
        (*vF)(Ym1,Yh);      Yh *= v1*h;            // df/dt at F_{n-1}
        Y += Yh;
        (*vF)(Yn,Yh);       Yh *= v0*h;            // df/dt at F_{n}
        Y += Yh;

        (*cF)(Yn, Yh, time, (c0*h));    Y += Yh;

        Ym3 = Ym2;          // Update n - 3
        Ym2 = Ym1;          // Update n - 2
        Ym1 = Yn;           // Update n - 1
        Yn = Y;            // Solution!

//      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        return Y;
    }
}
//**************************************************************



#endif

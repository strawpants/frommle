/*!@file blasoverload.hpp
@brief Overloads blas routines such that they can be called without  d,c,z, etc
@copyright Roelof Rietbroek 2014 (Dec 9, 2014)
@license
 This file is part of Frommle and is for private use of the author(s). The author(s) have not
 yet granted permission to publish the software under the GPL license.
 Redistribution of this software is prohibited.

Note this wrapper requires the cblas interface
*/

#include <core/F77calling.hpp>
#include <core/CoreTypes.hpp>
#ifndef GEOWBLASOVERLOAD
#define GEOWBLASOVERLOAD


///@brief Fortran BLAS subroutines are declared here
extern "C" {

void F77CALL(DCOPY,dcopy)(const F77Int &n , const F77Double &dx, const F77Int &incx , const F77Double &dy , const F77Int & incy );
void F77CALL(SCOPY,scopy)(const F77Int &n , const F77Float &dx, const F77Int &incx , const F77Float &dy , const F77Int & incy );
void F77CALL(ICOPY,icopy)(const F77Int &n , const F77Int &dx, const F77Int &incx , const F77Int &dy , const F77Int & incy );



}




//Overload BLAS functions with (simple?) inlines
namespace frommle {
	namespace core {

		inline void blas_copy(const int n, const double &dx, const int incx, const double &dy, const int incy) {
			F77CALL(DCOPY, dcopy)(n, dx, incx, dy, incy);
		}

		inline void blas_copy(const int n, const float &dx, const int incx, const float &dy, const int incy) {
			F77CALL(SCOPY, scopy)(n, dx, incx, dy, incy);
		}

		inline void blas_copy(const int n, const F77Int &dx, const int incx, const F77Int &dy, const int incy) {
			F77CALL(ICOPY, scopy)(n, dx, incx, dy, incy);
		}


//		inline void blas_copy(const int n, const Uint &dx, const int incx, const Uint &dy, const int incy) {
//			assert(0); //this routine should not be called currently as there is no blas equivalent for copying long long int
//		}
//
//		inline void blas_copy(const int n, const Int &dx, const int incx, const Int &dy, const int incy) {
//			assert(0); //this routine should not be called currently as there is no blas equivalent for copying long long int
//		}

	//matmul (gemm) family
//		inline void matmul(const char transa,const char transb, dgemm (TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
	}
}


#endif /* GEOWBLASOVERLOAD */

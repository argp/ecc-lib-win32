//    This file is part of ecc-lib-2.0.
//
//    ecc-lib-2.0 is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    ecc-lib-2.0 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with ecc-lib-2.0; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


// -----------------------------------------------------------------------------
//
//  File:        complex_arithmetic.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Complex Arithmetic
//
//  (C) 2003, Elisavet Konstantinou & Yiannis Stamatiu & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// -----------------------------------------------------------------------------


#ifndef COMPLEX_ARITHMETICH
#define COMPLEX_ARITHMETICH


/* print a complex number u (real part u[0], imaginary part u[1]) */
void print_complex(mpf_t *u);


/* the following functions implement complex arithmetic */

/* w = abs(u) */
void cabsolute(mpf_t *u, mpf_t *w);


/* addition w = u + v */
void cadd(mpf_t *u, mpf_t *v, mpf_t *w);


/* subtraction w = u - v */
void csub(mpf_t *u, mpf_t *v, mpf_t *w);


/* division w = u/v */
void cdiv(mpf_t *u, mpf_t *v, mpf_t *w);


/* multiplication w = u*v */
void cmul(mpf_t *u, mpf_t *v, mpf_t *w);


/* w is the conjugate of u */
void cconj(mpf_t *u, mpf_t *w);


/* w = sqrt(u) */
void csqrt(mpf_t *u, mpf_t *w);

/* w = sqrt(u), used in weber,c */
void csqrt1(mpf_t *u, mpf_t *w);


/* w = exp(u) */
void cexp(mpf_t *u, mpf_t *w);


/* w = ln(u) */
void clog(mpf_t *u, mpf_t *w);


/* w = u^v */
void cpow(mpf_t *u, mpf_t *v, mpf_t *w);

/* print a polynomial with complex coefficients */
void print_poly( mpf_t *h, long p);


/* multiply two complex polynomials a and b with degrees m and n respectively and put
   the result in polynomial c */
void cpoly_mul(long m, long n, mpf_t *a, mpf_t *b, mpf_t *c, long *p);

#endif


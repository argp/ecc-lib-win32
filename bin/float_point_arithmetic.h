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
//  File:        float_point_arithmetic.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Floating point arithmetic
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
//  The functions computeln2(), myln(), mysin() and mycos() have been
//  adapted from the source code of the NTL library
// -----------------------------------------------------------------------------


#ifndef FLOAT_POINT_ARITHMETICH
#define FLOAT_POINT_ARITHMETICH


/* w = round(x) */
void round(mpf_t *x, mpf_t *w);


/* calculate w = u modulo p, where u and w floating point numbers */
void mymod(mpf_t *w, mpf_t *u, mpf_t *p);


/* calculate w = number! */
void prod(mpf_t *w, long number);


/* calculate w = exp(power) using the taylor series */
void myexp(mpf_t *w, mpf_t *power);


/* compute ln2 */
void computeln2(mpf_t *w);


/* calculate w = ln(u) using the taylor series */
void myln(mpf_t *w, mpf_t *x);


/* calculate w = cos(x) using the taylor series */
void mysin(mpf_t *w, mpf_t *x);


/* calculate w = sin(x) using the taylor series */
void mycos(mpf_t *w, mpf_t *x);


/* calculate w = arctan(x) using the taylor series */
void myarctan(mpf_t *w, mpf_t *x);


/* calculate w = arctan2(x,y) */
void myarctan1(mpf_t *w, mpf_t *x, mpf_t *y);


/* calculate w = arctan2(x,y) */
void myarctan2(mpf_t *w, mpf_t *y, mpf_t *x);


/* compute the greatest common divisor of three floating point numbers x1, x2 and x3 */
void gcd3(mpf_t *x1, mpf_t *x2, mpf_t *x3, mpf_t *result);

#endif


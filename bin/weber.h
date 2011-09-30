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
//  File:        weber.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Construction of Weber polynomials
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// -----------------------------------------------------------------------------



#ifndef WEBERH
#define WEBERH


/* the following functions implement the routines described in IEEEP1363 Standard */

void G(mpf_t *Dpos, mpf_t *result);


void I(mpf_t *Dpos, mpf_t *result);


void J1(mpf_t *A, mpf_t *C, mpf_t *result);


void K(mpf_t *Dpos, mpf_t *result);


void L(mpf_t *Dpos, mpf_t *A, mpf_t *C, mpf_t *result);


void M(mpf_t *A, mpf_t *C, mpf_t *result);


void N(mpf_t *Dpos, mpf_t *A, mpf_t *C, mpf_t *result);


/* w = F(q) */
void F(mpf_t *q, mpf_t *w);


/* this function returns the same output as F but it uses
   algorithm 7.8 from Harald Baier's PhD thesis */
void F_baier(mpf_t *q, mpf_t *w);


/* result = thita(D, A, B) */
void thita(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


void f0(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


/* this function returns the same output as F but it uses
   algorithm 7.10 from Harald Baier's PhD thesis */
void f0_baier(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


void f1(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


/* this function returns the same output as F but it uses
   algorithm 7.9 from Harald Baier's PhD thesis */
void f1_baier(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


void f2(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


/* this function returns the same output as F but it uses
   an algorithm described in Harald Baier's PhD thesis, chapter 7.2.2 */
void f2_baier(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *result);


void Cweber(mpf_t *Dpos, mpf_t *A, mpf_t *B, mpf_t *C, mpf_t *result);


/* compute the Weber polynomial P and its degree dP using the discriminant D */
void Weber(mpf_t *D, mpz_t *P, long *dP);


/* compute the Weber polynomial Pnew and its degree dPnew using the discriminant Dnew */
void final_weber(long Dnew, mpz_t *Pnew, long *dPnew);

#endif


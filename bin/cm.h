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
//  File:        cm.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Complex Multiplication method
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// -----------------------------------------------------------------------------




#ifndef CMH
#define CMH


#define DEBUG

gmp_randstate_t stat;


/* check if the discriminant D has a proper value */
int checkD( long D );


/* this test is used in Cornacchia's algorithm */
int square_test(mpz_t *zn, mpz_t *zq);


/* return 1 if a solution (zx, zy) to the diophantine equation
   with inputs the discriminant zd and the prime zp was found
   and 0 otherwise */
int Cornacchia(mpz_t *zd, mpz_t *zp, mpz_t *zx, mpz_t *zy);


/* check if m is a suitable elliptic curve order */
int check_m(mpz_t *m, mpz_t *p);
  

/* find a suitable order m with its corresponding order p of the 
   finite field F_p using the discriminant D */
void find_m(mpz_t *p, mpz_t *m, mpz_t *D);


/* find a non-residue number x modulo p */
void find_nonresidue(mpz_t *p, mpz_t *x);


/* convert a root of the Weber polynomial to a root of its corresponding
   Hilbert polynomial */
void Weber_to_Hilbert( long D, mpz_t *rootW, mpz_t *p, mpz_t *rootH);


/* find the elliptic curve coefficients a and b having as inputs the root zroot[rootnum] of, 
   the Hilbert polynomial, the discriminant D, the order zp and the elliptic curve order m */
void choose_ab( long D, long rootnum, mpz_t *zroot, mpz_t *zp, mpz_t *m, mpz_t *a, mpz_t *b);


/* find the elliptic curve coefficients a and b having as inputs the root zroot[rootnum] of 
   the Weber polynomial, the discriminant D, the order zp and the elliptic curve order m */
void choose_ab_weber( long D, long rootnum, mpz_t *zroot, mpz_t *zp, mpz_t *m, mpz_t *a, mpz_t *b);


/* return the order p1 of the finite field, the elliptic curve curv and its order m1
   having as input the discriminant D using Hilbert polynomials */
void CMmethod(int D, mpz_t *p1, mpz_t *m1, mpz_t *curv);


/* return the order p1 of the finite field, the elliptic curve curv and its order m1
   having as input the discriminant D using Weber polynomials */
void CMmethod_weber(int D, mpz_t *p1, mpz_t *m1, mpz_t *curv);

#endif


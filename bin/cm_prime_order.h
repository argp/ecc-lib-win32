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


// ------------------------------------------------------------------------------
//
//  File:        cm_prime_order.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Generation of prime order ECs with Complex Multiplication method
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// ------------------------------------------------------------------------------


#ifndef CM_PRIME_ORDERH
#define CM_PRIME_ORDERH


#define DEBUG


/* check if the discriminant D is congruent to 3 mod 8
   only these values of discriminant can generate prime order ECs */
int checkD_prime_order( long D );


/* return 1 if a solution (zx, zy) to the diophantine equation
   with inputs the discriminant zd1 and the prime zp was found
   and 0 otherwise. This algorithm is used only when the discriminant
   is congruent to 3 mod 8 */
int modified_Cornacchia(mpz_t *zd1, mpz_t *zp, mpz_t *zx, mpz_t *zy);


/* calculate the transpose of a matrix T 2x2 [T[o] T[1]] [T[2] T[3]]*/
void transpose_matrix(mpz_t T[4], mpz_t Tt[4]);


/* calculate the inverse of a matrix T 2x2, which has Delta = 1 */
void inverse_matrix(mpz_t T[4], mpz_t Tinv[4]);


/* calculate the multiplication of T 2x2 with U 2x1*/
void matrix_multU(mpz_t T[4], mpz_t U[2], mpz_t result[4]);


/* calculate the multiplication of T 2x2 with S 2x2*/
void matrix_multS(mpz_t T[4], mpz_t S[4], mpz_t result[4]);


/* return a prime p and an odd integer t such that the Diophantine equation
   4p = t^2 + Dy^2 holds, using an algorithm from IEEE Standard P1363 */
void find_p_ieee(mpz_t *p, mpz_t *t, mpz_t *D);


/* return a prime p and an odd integer t such that the Diophantine equation
   4p = t^2 + Dy^2 holds, using the modified cornacchia's algorithm  */
void find_p_with_modified_cornacchia(mpz_t *p, mpz_t *t, mpz_t *D);


/* return a prime p and an odd integer t such that the Diophantine equation
   4p = t^2 + Dy^2 holds and p is between 2^(bitlength-1) and 2^bitlength */
void find_p_in_interval(mpz_t *p, mpz_t *t, mpz_t *D);


/* return a prime p and an odd integer t such that the Diophantine equation
   4p = t^2 + Dy^2 holds, by generating t and y randomly */
void find_p_randomly(mpz_t *p, mpz_t *t, mpz_t *D);


/* return the appropriate value of Table 4.7 in Harald Baier's PhD thesis.
   this fuction is used in find_p_and_m_baier() */
int cycles5mod8(int t3, int t5, int t7);


/* return 1 if there is no zero in the array, 0 otherwise */
int check_for_zeros(int a[26]);


/* find a prime order m of an elliptic curve defined in the finite field
   F_p, the order p of the finite field F_p and t such that
   the Diophantine equation 4p = t^2 + Dy^2 holds, using the algorithm
   4.7 from Harald Baier's PhD thesis */
void find_p_and_m_baier(mpz_t *m, mpz_t *p, mpz_t *t, mpz_t *D);


/*p, t and D are inputs, m is the output*/
/*the function returns 1 if m is prime and 0 otherwise*/
int find_prime_m(mpz_t *p, mpz_t *t, mpz_t *m, mpz_t *D);


/* return the order p1 of the finite field, the elliptic curve curv and its prime order m1
   having as input the discriminant D */
void CMmethod_prime_order(int D, mpz_t *p1, mpz_t *m1, mpz_t *curv);



#endif


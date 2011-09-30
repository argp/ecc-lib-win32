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
//  File:        hilbert.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Construction of Hilbert polynomials
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// -----------------------------------------------------------------------------


#ifndef HILBERTH
#define HILBERTH


void Delta(mpf_t *q, mpf_t *w);


void jf(mpf_t *tau, mpf_t *w);


void Hilbert(mpf_t *D, mpz_t *P, long *dP);


/* generates the Hilbert polynomials, input is Dnew and output
   the polynomial Pnew and its degree dPnew */
void final_hilbert(long Dnew, mpz_t *Pnew, long *dPnew);

#endif


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


// -----------------------------------------------------------------------------------
//
//  File:        create_e_and_pi.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Create the irrational numbers e and pi
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
//  The following functions have been adapted from the source code of the NTL library
// -----------------------------------------------------------------------------------


#ifndef CREATE_E_AND_PIH
#define CREATE_E_AND_PIH



/* creates e */
void create_e(mpf_t *e);


/* creates pi */
void create_pi(mpf_t *pi);


#endif /* CREATE_E_AND_PIH */


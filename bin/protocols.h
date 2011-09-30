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
//  File:        protocols.h
//  Date:        11/03
//  Last update: 11/03
//  Description: Cryptographic Protocols (Encryption-Decryption-Signature)
//
//  (C) 2003, Elisavet Konstantinou & Yannis Stamatiou & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// -----------------------------------------------------------------------------


#ifndef PROTOCOLSH
#define PROTOCOLSH


    
/* return a mask (length oLen) using the message Z which has length zLen */
void MGF_Hash(unsigned char *Z, unsigned long zLen, unsigned long oLen, unsigned char *mask);


/* encrypt the message (which has length message_len) using the point shared_key. The encrypted 
   message is the encoded_message. This function implements the one time pad algorithm. So, it 
   generates a mask which has the same length as the message and the encrypted message is the
   result of the xor between the mask and the message. To retrieve the message from the encrypted
   one, it followns the same process (generates the mask and xor with the encrypted message). So
   this function is also used for the decryption of messages with the difference that the input
   will be the encrypted message and output the original message */
void encryption( unsigned char *message, unsigned long message_len, mpz_t *shared_key, unsigned char *encoded_message);


/* encrypt a file filein using the shared_key and put the result in the
   file fileout. For decryption use as input the encrypted file and
   the output will be the original file */
int file_encryption(char *filein, mpz_t *shared_key, char *fileout);


/* transform a message to a large integer */
void message_to_int(unsigned char *message, mpz_t *result);


/* transform a large integer r to a string using the inverse transformation of the 
   above function */
void int_to_message(mpz_t *r, unsigned char *message);


/* encrypt a message. The result of the encryption is a point R and an integer c. Inputs
   are the elliptic curve curv, the prime p, the large prime factor n of the order of
   the elliptic curve, the base point base and the public key of the user */
void encryption_system(unsigned char *message, mpz_t *curv, mpz_t *p, mpz_t *n, mpz_t *base, mpz_t *publicB, mpz_t *R,
mpz_t *c);


/* using the above encryption method, it encrypts a file (filein). The result is placed
  in the file fileout */
int file_encryption_system(char *filein, mpz_t *curv, mpz_t *p, mpz_t *n, mpz_t *base, mpz_t *publicB, mpz_t *R,
mpz_t *c, char *fileout);


/* retrieves the message from the point R and the integer c, which were produced from 
   the function encryption_system. For the decryption is used the corresponding private
   key to the public key used in encryption */
void decryption_system(unsigned char *message, mpz_t *curv, mpz_t *p, mpz_t *privateB, mpz_t *R,
mpz_t *c);


/* using the previous decryption function, it decrypts the file filein (which must have the
   form of the file fileout of the function file_encryption_system) and puts the original
   file to fileout */
int file_decryption_system(char *filein, mpz_t *curv, mpz_t *p, mpz_t *privateB, mpz_t *R,
mpz_t *c, char *fileout);


/* generate a pair of integers (r, s) which represents the signature on the message. d is the
   private key of the signer */
void create_signature(char *message, mpz_t *curv, mpz_t *base_point, mpz_t *p, mpz_t *n, mpz_t *d, mpz_t *r, mpz_t *s);


/* verify the signature (r, s) using the public key Q of the signer. Returns 1 if the
   signature is verified and 0 otherwise */
int verify_signature(char *message, mpz_t *curv, mpz_t *base_point, mpz_t *p, mpz_t *n, mpz_t *Q, mpz_t *r, mpz_t *s);
 

/* generate a signature on the file filein and put the result in file fileout */
int file_create_signature(char *filein, mpz_t *curv, mpz_t *base_point, mpz_t *p, mpz_t *n, mpz_t *d, char *fileout);


/* verify the signature in file filein which was created on the file fileout. Returns 1 if 
   the signature was verified and 0 otherwise */
int file_verify_signature(char *filein, mpz_t *curv, mpz_t *base_point, mpz_t *p, mpz_t *n, mpz_t *Q, char *fileout);

#endif


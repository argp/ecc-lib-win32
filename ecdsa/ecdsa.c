/*
 * NTRG ECC-LIB WIN32 -- ecdsa.c
 * Patroklos G. Argyroudis <argp@cs.tcd.ie>
 *
 * Modified version of ecdsa.c for compilation of ECC-LIB on Win32.
 *
 * $Id: ecdsa.c,v 1.1.1.1 2004/01/11 10:24:10 argp Exp $
 */

#include <stdio.h>
#include <stdlib.h>
/* #include <unistd.h> */

#include "gmp.h"

#include "cm.h"
#include "ec_operations.h"
#include "protocols.h"

gmp_randstate_t stat;

int main(void)
{
     /* Initialization */
      long D, sd = 0;
      
      mpz_t p, m, n, h, private_key;
      mpz_t curv[2], base_point[2], public_key[2];
      mpz_t seed;
      
      gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
     
      mpz_init(p);
      mpz_init(m);
      mpz_init(n);
      mpz_init(h);
      mpz_init(private_key);
      mpz_init(curv[0]); mpz_init(curv[1]);
      mpz_init(base_point[0]); mpz_init(base_point[1]);
      mpz_init(public_key[0]); mpz_init(public_key[1]);
      mpz_init(seed);

      srand( (unsigned) getpid());
      sd=rand();
      mpz_set_ui(seed, sd);
  
      gmp_randseed(stat, seed);


      /* set the value of the discriminant*/
      D = 40;

      /* create the order p, the elliptic curve and its order m using D */
      CMmethod(D, &p, &m, curv);

      /* create a base point of order n, where m = nh and returns n and h*/
      domain_parameters(curv, base_point, &p, &m, &n, &h);

      /* create a private and a public key */
      create_priv_and_public(curv, &p, base_point, &private_key, public_key);

      /* generate the signature on the message using ECDSA */
      file_create_signature("README", curv, base_point, &p, &n, &private_key, "signature");
  
      gmp_randclear(stat);
      mpz_clear(p);
      mpz_clear(m);
      mpz_clear(n);
      mpz_clear(h);
      mpz_clear(private_key);
      mpz_clear(curv[0]); mpz_clear(curv[1]);
      mpz_clear(base_point[0]); mpz_clear(base_point[1]);
      mpz_clear(public_key[0]); mpz_clear(public_key[1]);
      mpz_clear(seed);

      return 0;
}

/* EOF */

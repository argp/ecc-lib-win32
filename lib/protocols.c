/*
 * NTRG ECC-LIB WIN32 -- protocols.c
 * Patroklos G. Argyroudis <argp@cs.tcd.ie>
 *
 * Modified version of protocols.c for compilation of ECC-LIB on Win32.
 *
 * $Id: protocols.c,v 1.1.1.1 2004/01/11 10:24:10 argp Exp $
 */

// --------------------------------------------------------------------
//
//  File:        protocols.c
//  Date:        11/03
//  Last update: 11/03
//  Description: Cryptographic Protocols (Encryption-Decryption-Signature)
//
//  (C) 2003, Elisavet Konstantinou & Yiannis Stamatiu & Christos Zaroliagis
//                 {konstane,stamatiu,zaro}@ceid.upatras.gr
//
// --------------------------------------------------------------------


# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "gmp.h"
# include "rmd160.h"
# include "int_arithmetic.h"
# include "ec_operations.h"

# include "protocols.h"


/* return a mask (length oLen) using the message Z which has length zLen */
void MGF_Hash(unsigned char *Z, unsigned long zLen, unsigned long oLen,
			  unsigned char *mask)
{
	/* unsigned char catspace[zLen + 4]; */
	unsigned char *catspace;
	unsigned char catpoint[4];

	int tbyte, counter = 0, i;
	unsigned long cThreshold = oLen / 20;
	unsigned char M[RMDsize / 8], temp[RMDsize / 8];

	catspace = (unsigned char *)malloc((sizeof(unsigned char) * (zLen + 4)));

//  establish enough memory space to do the hashing with counter

	for (i = 1; i < zLen + 1; i++) {
		catspace[i - 1] = Z[i - 1];
	}

	//  create main block of mask data
	if (cThreshold > 0) {

		for (counter = 0; counter < cThreshold; counter++) {
			*catpoint = (counter >> 24) & 0xFF;
			*(catpoint + 1) = (counter >> 16) & 0xFF;
			*(catpoint + 2) = (counter >> 8) & 0xFF;
			*(catpoint + 3) = counter & 0xFF;

			for (i = 1; i < 5; i++)
				catspace[zLen - 1 + i] = catpoint[i - 1];

			RMD(catspace, zLen + 4, M);

			for (i = 0; i < 20; i++)
				mask[i + counter * 20] = *(M + i);
		}
	}
//  finish up any uneven multiples of a single hash step
//       if ( cThreshold == 0) counter = 100;

	tbyte = oLen % 20;

	if (tbyte > 0) {
		*catpoint = (counter >> 24) & 0xFF;
		*(catpoint + 1) = (counter >> 16) & 0xFF;
		*(catpoint + 2) = (counter >> 8) & 0xFF;
		*(catpoint + 3) = counter & 0xFF;

		for (i = 1; i < 5; i++)
			catspace[zLen - 1 + i] = catpoint[i - 1];

		RMD(catspace, zLen + 4, temp);

		for (i = 0; i < 20; i++)
			mask[i + cThreshold * 20] = *(temp + i);
	}

	free(catspace);

}


/* encrypt the message (which has length message_len) using the point shared_key. The encrypted
   message is the encoded_message. This function implements the one time pad algorithm. So, it
   generates a mask which has the same length as the message and the encrypted message is the
   result of the xor between the mask and the message. To retrieve the message from the encrypted
   one, it followns the same process (generates the mask and xor with the encrypted message). So
   this function is also used for the decryption of messages with the difference that the input
   will be the encrypted message and output the original message */
void encryption(unsigned char *message, unsigned long message_len,
				mpz_t * shared_key, unsigned char *encoded_message)
{
	mpz_t help1;
	unsigned long len1 = 4 * (bitlength / 32 + 1);
	unsigned long len2 = len1 / 4;
	/* unsigned long p1[len2]; */
	/* unsigned char h[len1]; */
	unsigned long *p1;
	unsigned char *h;
	unsigned char mask[MAX_CHAR];
	int i, j;

	p1 = (unsigned long *)malloc((sizeof(unsigned long) * len2));
	h = (unsigned char *)malloc((sizeof(unsigned char) * len1));

	mpz_init(help1);

	mpz_set(help1, shared_key[0]);

	// put the variable help1 in an array of longs
	for (i = 0; i < len2; i++) {
		p1[i] = mpz_get_ui(help1);
		mpz_tdiv_q_2exp(help1, help1, 32);
	}

	// put an array of longs in an array of characters
	for (j = 0; j < len2; j++)
		for (i = 0; i < 4; i++) {
			h[4 * j + i] = p1[j] & 255;
			p1[j] = p1[j] << 1;
		}

	// create the mask
	MGF_Hash(h, len1, message_len, mask);

	// xor with the input
	for (i = 1; i < message_len + 1; i++)
		encoded_message[i - 1] = message[i - 1] ^ mask[i - 1];


	encoded_message[i - 1] = '\0';

	mpz_clear(help1);
	free(p1);
	free(h);

}


/* encrypt a file filein using the shared_key and put the result in the
   file fileout. For decryption use as input the encrypted file and
   the output will be the original file */
int file_encryption(char *filein, mpz_t * shared_key, char *fileout)
{
	int i, j;

	FILE *fpi;
	FILE *fpo;

	char c1;
	unsigned char ins[20];
	unsigned char outs[20];
	unsigned long mes_len;

	fpi = fopen(filein, "r+");

	if (fpi == NULL) {
		printf("file %s does not exist\n", filein);
		return 0;
	}

	fpo = fopen(fileout, "w+");

	if (fpo == NULL) {
		printf("couldn't open file %s\n", fileout);
		return 0;
	}

	i = 0;
	do {
		fscanf(fpi, "%s", ins);

		mes_len = strlen(ins);

		c1 = fgetc(fpi);

		if (c1 != EOF) {
			encryption(ins, mes_len, shared_key, outs);

			i++;

			j = fputs(outs, fpo);

			if (i % 10 != 0)
				fputc(' ', fpo);
			else
				fputc('\n', fpo);
		}

	} while (c1 != EOF);

	return 1;

	fclose(fpo);
	fclose(fpi);

}


/* transform a message to a large integer */
void message_to_int(unsigned char *message, mpz_t * result)
{
	int i;
	unsigned long length;
	mpz_t help, help1;

	mpz_init(help);
	mpz_init(help1);

	length = (unsigned long) strlen((unsigned char *) message);

	for (i = 0; i < length; i++) {
		mpz_ui_pow_ui(help1, 69, i);


		if (message[i] == 'a' || message[i] == 'A')
			mpz_mul_ui(help1, help1, (long) 1);
		if (message[i] == 'b' || message[i] == 'B')
			mpz_mul_ui(help1, help1, (long) 2);
		if (message[i] == 'c' || message[i] == 'C')
			mpz_mul_ui(help1, help1, (long) 3);
		if (message[i] == 'd' || message[i] == 'D')
			mpz_mul_ui(help1, help1, (long) 4);
		if (message[i] == 'e' || message[i] == 'E')
			mpz_mul_ui(help1, help1, (long) 5);
		if (message[i] == 'f' || message[i] == 'F')
			mpz_mul_ui(help1, help1, (long) 6);
		if (message[i] == 'g' || message[i] == 'G')
			mpz_mul_ui(help1, help1, (long) 7);
		if (message[i] == 'h' || message[i] == 'H')
			mpz_mul_ui(help1, help1, (long) 8);
		if (message[i] == 'i' || message[i] == 'I')
			mpz_mul_ui(help1, help1, (long) 9);
		if (message[i] == 'j' || message[i] == 'J')
			mpz_mul_ui(help1, help1, (long) 10);
		if (message[i] == 'k' || message[i] == 'K')
			mpz_mul_ui(help1, help1, (long) 11);
		if (message[i] == 'l' || message[i] == 'L')
			mpz_mul_ui(help1, help1, (long) 12);
		if (message[i] == 'm' || message[i] == 'M')
			mpz_mul_ui(help1, help1, (long) 13);
		if (message[i] == 'n' || message[i] == 'N')
			mpz_mul_ui(help1, help1, (long) 14);
		if (message[i] == 'o' || message[i] == 'O')
			mpz_mul_ui(help1, help1, (long) 15);
		if (message[i] == 'p' || message[i] == 'P')
			mpz_mul_ui(help1, help1, (long) 16);
		if (message[i] == 'q' || message[i] == 'Q')
			mpz_mul_ui(help1, help1, (long) 17);
		if (message[i] == 'r' || message[i] == 'R')
			mpz_mul_ui(help1, help1, (long) 18);
		if (message[i] == 's' || message[i] == 'S')
			mpz_mul_ui(help1, help1, (long) 19);
		if (message[i] == 't' || message[i] == 'T')
			mpz_mul_ui(help1, help1, (long) 20);
		if (message[i] == 'u' || message[i] == 'U')
			mpz_mul_ui(help1, help1, (long) 21);
		if (message[i] == 'v' || message[i] == 'V')
			mpz_mul_ui(help1, help1, (long) 22);
		if (message[i] == 'w' || message[i] == 'W')
			mpz_mul_ui(help1, help1, (long) 23);
		if (message[i] == 'x' || message[i] == 'X')
			mpz_mul_ui(help1, help1, (long) 24);
		if (message[i] == 'y' || message[i] == 'Y')
			mpz_mul_ui(help1, help1, (long) 25);
		if (message[i] == 'z' || message[i] == 'Z')
			mpz_mul_ui(help1, help1, (long) 26);

		if (message[i] == '0')
			mpz_mul_ui(help1, help1, (long) 27);
		if (message[i] == '1')
			mpz_mul_ui(help1, help1, (long) 28);
		if (message[i] == '2')
			mpz_mul_ui(help1, help1, (long) 29);
		if (message[i] == '3')
			mpz_mul_ui(help1, help1, (long) 30);
		if (message[i] == '4')
			mpz_mul_ui(help1, help1, (long) 31);
		if (message[i] == '5')
			mpz_mul_ui(help1, help1, (long) 32);
		if (message[i] == '6')
			mpz_mul_ui(help1, help1, (long) 33);
		if (message[i] == '7')
			mpz_mul_ui(help1, help1, (long) 34);
		if (message[i] == '8')
			mpz_mul_ui(help1, help1, (long) 35);
		if (message[i] == '9')
			mpz_mul_ui(help1, help1, (long) 36);

		if (message[i] == '.')
			mpz_mul_ui(help1, help1, (long) 37);
		if (message[i] == ',')
			mpz_mul_ui(help1, help1, (long) 38);
		if (message[i] == '/')
			mpz_mul_ui(help1, help1, (long) 39);
		if (message[i] == '\\')
			mpz_mul_ui(help1, help1, (long) 40);
		if (message[i] == ';')
			mpz_mul_ui(help1, help1, (long) 41);
		if (message[i] == '(')
			mpz_mul_ui(help1, help1, (long) 42);
		if (message[i] == ')')
			mpz_mul_ui(help1, help1, (long) 43);
		if (message[i] == '!')
			mpz_mul_ui(help1, help1, (long) 44);
		if (message[i] == '@')
			mpz_mul_ui(help1, help1, (long) 45);
		if (message[i] == '#')
			mpz_mul_ui(help1, help1, (long) 46);
		if (message[i] == '$')
			mpz_mul_ui(help1, help1, (long) 47);
		if (message[i] == '%')
			mpz_mul_ui(help1, help1, (long) 48);
		if (message[i] == '^')
			mpz_mul_ui(help1, help1, (long) 49);
		if (message[i] == '&')
			mpz_mul_ui(help1, help1, (long) 50);
		if (message[i] == '*')
			mpz_mul_ui(help1, help1, (long) 51);
		if (message[i] == '-')
			mpz_mul_ui(help1, help1, (long) 52);
		if (message[i] == '_')
			mpz_mul_ui(help1, help1, (long) 53);
		if (message[i] == '|')
			mpz_mul_ui(help1, help1, (long) 54);
		if (message[i] == ':')
			mpz_mul_ui(help1, help1, (long) 55);
		if (message[i] == '"')
			mpz_mul_ui(help1, help1, (long) 56);
		if (message[i] == '\'')
			mpz_mul_ui(help1, help1, (long) 57);
		if (message[i] == '?')
			mpz_mul_ui(help1, help1, (long) 58);
		if (message[i] == '<')
			mpz_mul_ui(help1, help1, (long) 59);
		if (message[i] == '>')
			mpz_mul_ui(help1, help1, (long) 60);
		if (message[i] == '~')
			mpz_mul_ui(help1, help1, (long) 61);
		if (message[i] == '=')
			mpz_mul_ui(help1, help1, (long) 62);
		if (message[i] == '+')
			mpz_mul_ui(help1, help1, (long) 63);
		if (message[i] == '{')
			mpz_mul_ui(help1, help1, (long) 64);
		if (message[i] == '}')
			mpz_mul_ui(help1, help1, (long) 65);
		if (message[i] == '[')
			mpz_mul_ui(help1, help1, (long) 66);
		if (message[i] == ']')
			mpz_mul_ui(help1, help1, (long) 67);
		if (message[i] == '`')
			mpz_mul_ui(help1, help1, (long) 68);

		mpz_add(help, help, help1);
	}

	mpz_set(*result, help);

	mpz_clear(help);
	mpz_clear(help1);

}


/* transform a large integer r to a string using the inverse transformation of the
   above function */
void int_to_message(mpz_t * r, unsigned char *message)
{
	int i = 0;
	mpz_t help, help1, h;

	mpz_init(help);
	mpz_init(help1);
	mpz_init(h);

	mpz_set_ui(h, 69);
	mpz_set(help, *r);

	mpz_mod(help1, help, h);


	while (mpz_sgn(help) > 0) {
		if (mpz_cmp_ui(help1, (long) 1) == 0)
			message[i] = 'a';
		if (mpz_cmp_ui(help1, (long) 2) == 0)
			message[i] = 'b';
		if (mpz_cmp_ui(help1, (long) 3) == 0)
			message[i] = 'c';
		if (mpz_cmp_ui(help1, (long) 4) == 0)
			message[i] = 'd';
		if (mpz_cmp_ui(help1, (long) 5) == 0)
			message[i] = 'e';
		if (mpz_cmp_ui(help1, (long) 6) == 0)
			message[i] = 'f';
		if (mpz_cmp_ui(help1, (long) 7) == 0)
			message[i] = 'g';
		if (mpz_cmp_ui(help1, (long) 8) == 0)
			message[i] = 'h';
		if (mpz_cmp_ui(help1, (long) 9) == 0)
			message[i] = 'i';
		if (mpz_cmp_ui(help1, (long) 10) == 0)
			message[i] = 'j';
		if (mpz_cmp_ui(help1, (long) 11) == 0)
			message[i] = 'k';
		if (mpz_cmp_ui(help1, (long) 12) == 0)
			message[i] = 'l';
		if (mpz_cmp_ui(help1, (long) 13) == 0)
			message[i] = 'm';
		if (mpz_cmp_ui(help1, (long) 14) == 0)
			message[i] = 'n';
		if (mpz_cmp_ui(help1, (long) 15) == 0)
			message[i] = 'o';
		if (mpz_cmp_ui(help1, (long) 16) == 0)
			message[i] = 'p';
		if (mpz_cmp_ui(help1, (long) 17) == 0)
			message[i] = 'q';
		if (mpz_cmp_ui(help1, (long) 18) == 0)
			message[i] = 'r';
		if (mpz_cmp_ui(help1, (long) 19) == 0)
			message[i] = 's';
		if (mpz_cmp_ui(help1, (long) 20) == 0)
			message[i] = 't';
		if (mpz_cmp_ui(help1, (long) 21) == 0)
			message[i] = 'u';
		if (mpz_cmp_ui(help1, (long) 22) == 0)
			message[i] = 'v';
		if (mpz_cmp_ui(help1, (long) 23) == 0)
			message[i] = 'w';
		if (mpz_cmp_ui(help1, (long) 24) == 0)
			message[i] = 'x';
		if (mpz_cmp_ui(help1, (long) 25) == 0)
			message[i] = 'y';
		if (mpz_cmp_ui(help1, (long) 26) == 0)
			message[i] = 'z';

		if (mpz_cmp_ui(help1, (long) 27) == 0)
			message[i] = '0';
		if (mpz_cmp_ui(help1, (long) 28) == 0)
			message[i] = '1';
		if (mpz_cmp_ui(help1, (long) 29) == 0)
			message[i] = '2';
		if (mpz_cmp_ui(help1, (long) 30) == 0)
			message[i] = '3';
		if (mpz_cmp_ui(help1, (long) 31) == 0)
			message[i] = '4';
		if (mpz_cmp_ui(help1, (long) 32) == 0)
			message[i] = '5';
		if (mpz_cmp_ui(help1, (long) 33) == 0)
			message[i] = '6';
		if (mpz_cmp_ui(help1, (long) 34) == 0)
			message[i] = '7';
		if (mpz_cmp_ui(help1, (long) 35) == 0)
			message[i] = '8';
		if (mpz_cmp_ui(help1, (long) 36) == 0)
			message[i] = '9';

		if (mpz_cmp_ui(help1, (long) 37) == 0)
			message[i] = '.';
		if (mpz_cmp_ui(help1, (long) 38) == 0)
			message[i] = ',';
		if (mpz_cmp_ui(help1, (long) 39) == 0)
			message[i] = '/';
		if (mpz_cmp_ui(help1, (long) 40) == 0)
			message[i] = '\\';
		if (mpz_cmp_ui(help1, (long) 41) == 0)
			message[i] = ';';
		if (mpz_cmp_ui(help1, (long) 42) == 0)
			message[i] = '(';
		if (mpz_cmp_ui(help1, (long) 43) == 0)
			message[i] = ')';
		if (mpz_cmp_ui(help1, (long) 44) == 0)
			message[i] = '!';
		if (mpz_cmp_ui(help1, (long) 45) == 0)
			message[i] = '@';
		if (mpz_cmp_ui(help1, (long) 46) == 0)
			message[i] = '#';
		if (mpz_cmp_ui(help1, (long) 47) == 0)
			message[i] = '$';
		if (mpz_cmp_ui(help1, (long) 48) == 0)
			message[i] = '%';
		if (mpz_cmp_ui(help1, (long) 49) == 0)
			message[i] = '^';
		if (mpz_cmp_ui(help1, (long) 50) == 0)
			message[i] = '&';
		if (mpz_cmp_ui(help1, (long) 51) == 0)
			message[i] = '*';
		if (mpz_cmp_ui(help1, (long) 52) == 0)
			message[i] = '-';
		if (mpz_cmp_ui(help1, (long) 53) == 0)
			message[i] = '_';
		if (mpz_cmp_ui(help1, (long) 54) == 0)
			message[i] = '|';
		if (mpz_cmp_ui(help1, (long) 55) == 0)
			message[i] = ':';
		if (mpz_cmp_ui(help1, (long) 56) == 0)
			message[i] = '"';
		if (mpz_cmp_ui(help1, (long) 57) == 0)
			message[i] = '\'';
		if (mpz_cmp_ui(help1, (long) 58) == 0)
			message[i] = '?';
		if (mpz_cmp_ui(help1, (long) 59) == 0)
			message[i] = '<';
		if (mpz_cmp_ui(help1, (long) 60) == 0)
			message[i] = '>';
		if (mpz_cmp_ui(help1, (long) 61) == 0)
			message[i] = '~';
		if (mpz_cmp_ui(help1, (long) 62) == 0)
			message[i] = '=';
		if (mpz_cmp_ui(help1, (long) 63) == 0)
			message[i] = '+';
		if (mpz_cmp_ui(help1, (long) 64) == 0)
			message[i] = '{';
		if (mpz_cmp_ui(help1, (long) 65) == 0)
			message[i] = '}';
		if (mpz_cmp_ui(help1, (long) 66) == 0)
			message[i] = '[';
		if (mpz_cmp_ui(help1, (long) 67) == 0)
			message[i] = ']';
		if (mpz_cmp_ui(help1, (long) 68) == 0)
			message[i] = '`';

		i++;

		mpz_sub(help, help, help1);
		mpz_tdiv_q(help, help, h);
		mpz_mod(help1, help, h);

	}

	message[i] = '\0';

	mpz_clear(help);
	mpz_clear(help1);
	mpz_clear(h);
}


/* encrypt a message. The result of the encryption is a point R and an integer c. Inputs
   are the elliptic curve curv, the prime p, the large prime factor n of the order of
   the elliptic curve, the base point base and the public key of the user */
void encryption_system(unsigned char *message, mpz_t * curv, mpz_t * p,
					   mpz_t * n, mpz_t * base, mpz_t * publicB, mpz_t * R,
					   mpz_t * c)
{
	mpz_t k2, k1;
	mpz_t Q[2];

	mpz_init(k2);
	mpz_init(k1);

	mpz_init(Q[0]);
	mpz_init(Q[1]);


	do {

		myprimegenerator(&k2);
		mpz_mod(k1, k2, *n);
		mpz_sub_ui(k2, *n, 1);

	} while (mpz_cmp_ui(k1, (long) 1) == 0 || mpz_cmp(k1, k2) == 0);

	point_mult(curv, base, &k1, R, p);
	point_mult(curv, publicB, &k1, Q, p);

	message_to_int(message, &k2);

	if (mpz_cmp(k2, *p) > 0) {
		printf("Error:Message is bigger than order!!\n");

	}

	myzmulmod(c, &k2, &Q[0], p);


	mpz_clear(k2);
	mpz_clear(k1);

	mpz_clear(Q[0]);
	mpz_clear(Q[1]);

}


/* using the above encryption method, it encrypts a file (filein). The result is placed
  in the file fileout */
int file_encryption_system(char *filein, mpz_t * curv, mpz_t * p, mpz_t * n,
						   mpz_t * base, mpz_t * publicB, mpz_t * R,
						   mpz_t * c, char *fileout)
{
	int i, j, k;
	unsigned char help_mes[20];

	FILE *fpi;
	FILE *fpo;

	char c1;
	unsigned char *ins =
		(unsigned char *) malloc(sizeof(unsigned char) * (MAX_CHAR + 1));


	fpi = fopen(filein, "r+");

	if (fpi == NULL) {
		printf("file %s does not exist\n", filein);
		return 0;
	}

	fpo = fopen(fileout, "w+");
	if (fpo == NULL) {
		printf("couldn't open file %s\n", fileout);
		return 0;
	}

	do {
		fscanf(fpi, "%s", ins);
		c1 = fgetc(fpi);

		if (c1 != EOF) {

			if (strlen(ins) > 20) {
				j = strlen(ins) / 20;

				for (k = 0; k < j; k++) {
					for (i = 0; i < 20; i++)
						help_mes[i] = ins[i + 20 * k];

					encryption_system(help_mes, curv, p, n, base, publicB, R,
									  c);

					mpz_out_str(fpo, 10, R[0]);
					fputc(' ', fpo);
					mpz_out_str(fpo, 10, R[1]);
					fputc(' ', fpo);
					mpz_out_str(fpo, 10, *c);
					fputc(' ', fpo);
				}

				for (i = j * 20; i < strlen(ins); i++)
					help_mes[i - j * 20] = ins[i];
				help_mes[i - j * 20] = '\0';

				encryption_system(help_mes, curv, p, n, base, publicB, R, c);

				mpz_out_str(fpo, 10, R[0]);
				fputc(' ', fpo);
				mpz_out_str(fpo, 10, R[1]);
				fputc(' ', fpo);
				mpz_out_str(fpo, 10, *c);
				fputc('\n', fpo);

			}

			else {
				encryption_system(ins, curv, p, n, base, publicB, R, c);

				mpz_out_str(fpo, 10, R[0]);
				fputc(' ', fpo);
				mpz_out_str(fpo, 10, R[1]);
				fputc(' ', fpo);
				mpz_out_str(fpo, 10, *c);
				fputc(c1, fpo);
			}
		}

	} while (c1 != EOF);


	fclose(fpo);
	fclose(fpi);

	free(ins);

	return 1;
}


/* retrieves the message from the point R and the integer c, which were produced from
   the function encryption_system. For the decryption is used the corresponding private
   key to the public key used in encryption */
void decryption_system(unsigned char *message, mpz_t * curv, mpz_t * p,
					   mpz_t * privateB, mpz_t * R, mpz_t * c)
{
	mpz_t m;
	mpz_t Q[2];

	mpz_init(m);
	mpz_init(Q[0]);
	mpz_init(Q[1]);

	point_mult(curv, R, privateB, Q, p);

	myzdivmod(&m, c, &Q[0], p);

	int_to_message(&m, message);

	mpz_clear(m);
	mpz_clear(Q[0]);
	mpz_clear(Q[1]);
}


/* using the previous decryption function, it decrypts the file filein (which must have the
   form of the file fileout of the function file_encryption_system) and puts the original
   file to fileout */
int file_decryption_system(char *filein, mpz_t * curv, mpz_t * p,
						   mpz_t * privateB, mpz_t * R, mpz_t * c,
						   char *fileout)
{
	int i;

	char c1;
	FILE *fpi;
	FILE *fpo;

	unsigned char *outs =
		(unsigned char *) malloc(sizeof(unsigned char) * (MAX_CHAR + 1));

	fpi = fopen(filein, "r+");

	if (fpi == NULL) {
		printf("file %s does not exist\n", filein);
		return 0;
	}

	fpo = fopen(fileout, "w+");

	if (fpo == NULL) {
		printf("couldn't write in file %s\n", fileout);
		return 0;
	}

	i = 0;

	do {
		i++;

		mpz_inp_str(R[0], fpi, 10);
		mpz_inp_str(R[1], fpi, 10);

		mpz_inp_str(*c, fpi, 10);

		c1 = fgetc(fpi);

		if (c1 != EOF) {
			decryption_system(outs, curv, p, privateB, R, c);

			fprintf(fpo, "%s", outs);
			fputc(c1, fpo);
		}

	} while (c1 != EOF);


	fclose(fpo);
	fclose(fpi);

	free(outs);

	return 1;
}


/* generate a pair of integers (r, s) which represents the signature on the message. d is the
   private key of the signer */
void create_signature(char *message, mpz_t * curv, mpz_t * base_point,
					  mpz_t * p, mpz_t * n, mpz_t * d, mpz_t * r, mpz_t * s)
{
	int i;
	mpz_t x1;
	mpz_t k1, k, e;
	mpz_t p1[2];
	mpz_t t;

	unsigned char hashcode[RMDsize / 8];
	unsigned long test[RMDsize / 8];

	mpz_init(x1);
	mpz_init(k1);
	mpz_init(k);
	mpz_init(e);
	mpz_init(p1[0]);
	mpz_init(p1[1]);
	mpz_init(t);

	mpz_set_ui(*r, 0);

	while (mpz_sgn(*r) == 0) {
		myprimegenerator(&k1);

		mpz_mod(k, k1, *n);

		if (mpz_sgn(k) == 0)
			mpz_set_ui(k, 1);

		point_mult(curv, base_point, &k, p1, p);
		mpz_mod(*r, p1[0], *n);
	}

	mpz_invert(k1, k, *n);

	RMD((unsigned char *) message, strlen(message), hashcode);

	for (i = 1; i < RMDsize / 8 + 1; i++) {
		test[i - 1] = (unsigned long) hashcode[i - 1];
		mpz_set_ui(t, test[i - 1]);
		mpz_mul_ui(e, e, 256);
		mpz_add(e, e, t);
	}


	mpz_mul(x1, *d, *r);
	mpz_add(x1, x1, e);
	mpz_mul(*s, x1, k1);
	mpz_mod(*s, *s, *n);


	mpz_clear(x1);
	mpz_clear(k1);
	mpz_clear(k);
	mpz_clear(e);
	mpz_clear(p1[0]);
	mpz_clear(p1[1]);
	mpz_clear(t);
}


/* verify the signature (r, s) using the public key Q of the signer. Returns 1 if the
   signature is verified and 0 otherwise */
int verify_signature(char *message, mpz_t * curv, mpz_t * base_point,
					 mpz_t * p, mpz_t * n, mpz_t * Q, mpz_t * r, mpz_t * s)
{
	int r1, i;

	unsigned char hashcode[RMDsize / 8];
	unsigned long test[RMDsize / 8];

	mpz_t e, w;
	mpz_t u1, u2, u;
	mpz_t G1[2];
	mpz_t Q1[2];
	mpz_t X[2];
	mpz_t t;

	mpz_init(e);
	mpz_init(w);
	mpz_init(u1);
	mpz_init(u2);
	mpz_init(u);
	mpz_init(G1[0]);
	mpz_init(G1[1]);
	mpz_init(Q1[0]);
	mpz_init(Q1[1]);
	mpz_init(X[0]);
	mpz_init(X[1]);
	mpz_init(t);

	if (mpz_cmp_ui(*r, (long) 1) < 0 || mpz_cmp(*n, *r) < 0)
		return (0);

	if (mpz_cmp_ui(*s, (long) 1) < 0 || mpz_cmp(*n, *s) < 0)
		return (0);


	RMD((unsigned char *) message, strlen(message), hashcode);

	for (i = 1; i < RMDsize / 8 + 1; i++) {
		test[i - 1] = (unsigned long) hashcode[i - 1];
		mpz_set_ui(t, test[i - 1]);
		mpz_mul_ui(e, e, 256);
		mpz_add(e, e, t);
	}

	mpz_invert(w, *s, *n);

	myzmulmod(&u1, &e, &w, n);
	myzmulmod(&u2, r, &w, n);

	point_mult(curv, base_point, &u1, G1, p);
	point_mult(curv, Q, &u2, Q1, p);

	add_point(curv, G1, Q1, X, p);

	if (mpz_sgn(X[0]) == 0 && mpz_sgn(X[1]) == 0)
		return (0);

	mpz_mod(u, X[0], *n);

	if (mpz_cmp(u, *r) == 0)
		r1 = 1;

	else
		r1 = 0;


	mpz_clear(e);
	mpz_clear(w);
	mpz_clear(u1);
	mpz_clear(u2);
	mpz_clear(u);
	mpz_clear(G1[0]);
	mpz_clear(G1[1]);
	mpz_clear(Q1[0]);
	mpz_clear(Q1[1]);
	mpz_clear(X[0]);
	mpz_clear(X[1]);
	mpz_clear(t);

	return (r1);
}


/* generate a signature on the file filein and put the result in file fileout */
int file_create_signature(char *filein, mpz_t * curv, mpz_t * base_point,
						  mpz_t * p, mpz_t * n, mpz_t * d, char *fileout)
{
	FILE *fpi, *fpo;

	int i;
	mpz_t x1;
	mpz_t k1, k, e;
	mpz_t p1[2];
	mpz_t t;
	mpz_t r, s;

	unsigned char ins[MAX_CHAR];

	unsigned char hashcode[RMDsize / 8];
	unsigned long test[RMDsize / 8];

	mpz_init(x1);
	mpz_init(k1);
	mpz_init(k);
	mpz_init(e);
	mpz_init(p1[0]);
	mpz_init(p1[1]);
	mpz_init(t);
	mpz_init(r);
	mpz_init(s);


	mpz_set_ui(r, 0);

	while (mpz_sgn(r) == 0) {
		myprimegenerator(&k1);

		mpz_mod(k, k1, *n);

		if (mpz_sgn(k) == 0)
			mpz_set_ui(k, 1);

		point_mult(curv, base_point, &k, p1, p);
		mpz_mod(r, p1[0], *n);
	}

	mpz_invert(k1, k, *n);


	fpi = fopen(filein, "r+");

	if (fpi == NULL) {
		printf("file %s does not exist\n", filein);
		return 0;
	}

	fscanf(fpi, "%s", ins);

	while (fgetc(fpi) != EOF) {
		RMD(ins, strlen(ins), hashcode);
		fscanf(fpi, "%s", ins);


		if (strlen(ins) < 21) {
			for (i = 0; i < strlen(ins); i++)
				ins[i] = ins[i] ^ hashcode[i];
		}

		else {
			for (i = 0; i < RMDsize / 8; i++)
				ins[i] = ins[i] ^ hashcode[i];
		}

	}

	fclose(fpi);


	for (i = 1; i < RMDsize / 8 + 1; i++) {
		test[i - 1] = (unsigned long) hashcode[i - 1];
		mpz_set_ui(t, test[i - 1]);
		mpz_mul_ui(e, e, 256);
		mpz_add(e, e, t);
	}

	mpz_mul(x1, *d, r);
	mpz_add(x1, x1, e);
	mpz_mul(s, x1, k1);
	mpz_mod(s, s, *n);

	fpo = fopen(fileout, "w+");
	if (fpo == NULL) {
		printf("couldn't open file %s\n", fileout);
		return 0;
	}


	mpz_out_str(fpo, 10, r);
	fputc(' ', fpo);
	mpz_out_str(fpo, 10, s);

	fclose(fpo);

	mpz_clear(x1);
	mpz_clear(k1);
	mpz_clear(k);
	mpz_clear(e);
	mpz_clear(p1[0]);
	mpz_clear(p1[1]);
	mpz_clear(t);
	mpz_clear(r);
	mpz_clear(s);

	return 1;
}


/* verify the signature in file filein which was created on the file fileout. Returns 1 if
   the signature was verified and 0 otherwise */
int file_verify_signature(char *filein, mpz_t * curv, mpz_t * base_point,
						  mpz_t * p, mpz_t * n, mpz_t * Q, char *fileout)
{
	FILE *fpi, *fpo;

	int r1, i;

	unsigned char hashcode[RMDsize / 8];
	unsigned long test[RMDsize / 8];
	unsigned char ins[MAX_CHAR];

	mpz_t e, w;
	mpz_t u1, u2, u;
	mpz_t G1[2];
	mpz_t Q1[2];
	mpz_t X[2];
	mpz_t t;
	mpz_t r, s;

	mpz_init(e);
	mpz_init(w);
	mpz_init(u1);
	mpz_init(u2);
	mpz_init(u);
	mpz_init(G1[0]);
	mpz_init(G1[1]);
	mpz_init(Q1[0]);
	mpz_init(Q1[1]);
	mpz_init(X[0]);
	mpz_init(X[1]);
	mpz_init(t);
	mpz_init(r);
	mpz_init(s);


	fpo = fopen(fileout, "r+");
	if (fpo == NULL) {
		printf("couldn't open file %s\n", fileout);
		return 0;
	}

	mpz_inp_str(r, fpo, 10);
	mpz_inp_str(s, fpo, 10);

	fclose(fpo);


	if (mpz_cmp_ui(r, (long) 1) < 0 || mpz_cmp(*n, r) < 0)
		return (0);

	if (mpz_cmp_ui(s, (long) 1) < 0 || mpz_cmp(*n, s) < 0)
		return (0);


	fpi = fopen(filein, "r+");

	if (fpi == NULL) {
		printf("file %s does not exist\n", filein);
		return 0;
	}

	fscanf(fpi, "%s", ins);

	while (fgetc(fpi) != EOF) {
		RMD(ins, strlen(ins), hashcode);
		fscanf(fpi, "%s", ins);


		if (strlen(ins) < 21) {
			for (i = 0; i < strlen(ins); i++)
				ins[i] = ins[i] ^ hashcode[i];
		}

		else {
			for (i = 0; i < RMDsize / 8; i++)
				ins[i] = ins[i] ^ hashcode[i];
		}

	}

	fclose(fpi);

	for (i = 1; i < RMDsize / 8 + 1; i++) {
		test[i - 1] = (unsigned long) hashcode[i - 1];
		mpz_set_ui(t, test[i - 1]);
		mpz_mul_ui(e, e, 256);
		mpz_add(e, e, t);
	}

	mpz_invert(w, s, *n);

	myzmulmod(&u1, &e, &w, n);
	myzmulmod(&u2, &r, &w, n);

	point_mult(curv, base_point, &u1, G1, p);
	point_mult(curv, Q, &u2, Q1, p);

	add_point(curv, G1, Q1, X, p);

	if (mpz_sgn(X[0]) == 0 && mpz_sgn(X[1]) == 0)
		return (0);

	mpz_mod(u, X[0], *n);

	if (mpz_cmp(u, r) == 0)
		r1 = 1;

	else
		r1 = 0;


	mpz_clear(e);
	mpz_clear(w);
	mpz_clear(u1);
	mpz_clear(u2);
	mpz_clear(u);
	mpz_clear(G1[0]);
	mpz_clear(G1[1]);
	mpz_clear(Q1[0]);
	mpz_clear(Q1[1]);
	mpz_clear(X[0]);
	mpz_clear(X[1]);
	mpz_clear(t);
	mpz_clear(r);
	mpz_clear(s);

	return (r1);
}

/* EOF */

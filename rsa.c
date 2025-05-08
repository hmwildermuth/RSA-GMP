#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef __GNU_MP__
#include "gmp_stub.h"
#else
#include <gmp.h>
#endif
#include <ctype.h>
#include <unistd.h>

#define KEY_SIZE 512

struct return_struct {
	size_t count;
	char * ciph;
};

struct private_key {
	mpz_t n, d, e, p, q; // exponents, modulus, and factors
};

struct private_key
genKey(int keysize)
{
	struct private_key key;
	gmp_randstate_t state;
	mpz_t tmp, lambda;
	mpz_t p_n;
	mpz_t q_n;
	mpz_t gcd;
	mpz_init(key.n);
	mpz_init(key.d);
	mpz_init(key.p);
	mpz_init(key.q);
	mpz_init(p_n);
	mpz_init(q_n);
	mpz_init(gcd);
	mpz_init(lambda);
	mpz_init_set_str(key.e, "65537", 10); // constant e
	mpz_init(tmp);
	gmp_randinit_default(state);
	gmp_randseed_ui(state, time(NULL));

	// gen p and q
	mpz_urandomb(tmp, state, keysize/2);
	mpz_nextprime(key.p, tmp);
	mpz_urandomb(tmp, state, keysize/2);
	mpz_nextprime(key.q, tmp);

	// calculate lambda
	mpz_sub_ui(p_n, key.p, 1); // p_n = p-1
	mpz_sub_ui(q_n, key.q, 1); // q_n = q-1
	mpz_mul(lambda, p_n, q_n); // lambda = lcm((p-1),(q-1))
	mpz_gcd (gcd, lambda, key.e); // gcd = gcd(lambda,e)

	// make sure gcd of lambda and e is 1
	while(mpz_cmp_ui(gcd, 1) != 0) {
		mpz_add_ui(key.e, key.e, 2);
		mpz_gcd (gcd, lambda, key.e); // gcd = gcd(lambda,e)
	}

	if (mpz_invert(key.d, key.e, lambda) == 0) {
		printf("Failure inverting key.\n");
	}
	mpz_mul(key.n, key.p, key.q);

	printf("PRIVATE KEY:\n");
	gmp_printf ("%s: %Zd\n", "d", key.d);
	gmp_printf ("%s: %Zd\n", "e", key.e);
	gmp_printf ("%s: %Zd\n", "p", key.p);
	gmp_printf ("%s: %Zd\n", "q", key.q);
	gmp_printf ("%s: %Zd\n", "n", key.n);
	printf("\nPUBLIC KEY:\n");
	gmp_printf ("%s: %Zd\n", "e", key.e);
	gmp_printf ("%s: %Zd\n", "n", key.n);


	return key;
}

struct return_struct
RSAencrypt(char *plaintext, int length, mpz_t e, mpz_t n)
{
	mpz_t m;
	mpz_init(m);
	mpz_t c;
	mpz_init(c);
	struct return_struct ret;
	size_t count;

	mpz_import(m, length, 1, sizeof(plaintext[0]), 0, 0, plaintext);
	mpz_powm(c, m, e, n);

	count = (mpz_sizeinbase(c, 2) + (8*sizeof(plaintext[0]))-1)/(8*sizeof(plaintext[0]));
	ret.ciph = calloc(count, sizeof(plaintext[0]));

	mpz_export(ret.ciph, &ret.count, 1, sizeof(plaintext[0]), 0, 0, c);

	return ret;
}

int
main(int argc, char *argv[]) {
	int gflag = 0;
	int nflag = 0;
	int eflag = 0;
	int fflag = 0;
	int oflag = 0;
	char *fvalue = NULL;
	char *nvalue = NULL;
	char *ovalue = NULL;
	char *evalue = NULL;
	int c;
	struct private_key key;
	struct return_struct a;
	char * plaintext;
	FILE *fp;
	int size;

	opterr = 0;

	while ((c = getopt (argc, argv, "ge:n:f:o:")) != -1)
	switch (c) {
	case 'g':
		gflag = 1;
		break;
	case 'e':
		evalue = optarg;
		eflag = 1;
		break;
	case 'n':
		nvalue = optarg;
		nflag = 1;
		break;
	case 'f':
		fvalue = optarg;
		fflag = 1;
		break;
	case 'o':
		ovalue = optarg;
		oflag = 1;
		break;
	case '?':
		if (optopt == 'e' || optopt == 'n' || optopt == 'f')
		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
		else if (isprint (optopt))
		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		else
		fprintf (stderr,
					"Unknown option character `\\x%x'.\n",
					optopt);
		return 1;
	default:
		abort ();
	}
	if (gflag && (nflag || eflag)) {
		fprintf(stderr, "-g cannot be used with -n and -e\n");
		return -1;
	}
	if (nflag != eflag) {
		fprintf(stderr, "You must use -n and -e in conjunction to provide the key's modulus and exponent.\n");
		return -1;
	}
	if (nflag && !fflag) {
		fprintf(stderr, "-n and -e must be used with -f [filename]\n");
		return -1;
	}
	if (!(gflag || nflag)) {
		fprintf(stderr, "Either -g or -n and -e must be used\n");
		return -1;
	}

	if (gflag) {
		key = genKey(KEY_SIZE);
		if (!fflag) {
			return 0;
		}
		printf("Encrypting with your private key (d).\n");
	} else {
		if(mpz_init_set_str(key.d,evalue,10) != 0) {
			fprintf(stderr, "e value (%s) not a base 10 number\n", evalue);
			return -1;
		} else {
		}
		if(mpz_init_set_str(key.n,nvalue,10) != 0) {
			fprintf(stderr, "n value (%s) not a base 10 number\n", nvalue);
			return -1;
		}
	}
	// Finally done with command line options crap

	fp = fopen (fvalue,"r+b");
	if (fp!=NULL)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		plaintext = calloc(size, sizeof(char));
		fread(plaintext, sizeof(char), size, fp);
		fclose(fp);
	} else {
		fprintf(stderr, "Failure opening file: %s\n", fvalue);
		return -1;
	}

	a = RSAencrypt(plaintext, size, key.d, key.n);

	if (oflag) {
		fp = fopen (ovalue,"w");
		if (fp!=NULL)
		{
			fprintf(fp,"%s", a.ciph);
			fclose (fp);
			printf("Successfully wrote to %s.\n", ovalue);
		} else {
			fprintf(stderr, "Could not open output file: %s\n", ovalue);
		}
	} else {
		printf("%s\n", a.ciph);
	}

	//a = RSAencrypt(a.ciph, a.count, key.e, key.n);

	//printf("Plaintext: %s\n", a.ciph);

	return 0;
}

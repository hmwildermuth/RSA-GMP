#ifndef GMP_STUB_H
#define GMP_STUB_H

#include <stdint.h>

typedef struct { unsigned long _mp_alloc; unsigned long _mp_size; uint64_t *_mp_d; } __mpz_struct;
typedef __mpz_struct mpz_t[1];
typedef struct { int dummy; } gmp_randstate_t[1];

// Declare a handful of GMP functions used in rsa.c as no-op stubs to appease the compiler.
static inline void mpz_init(mpz_t x) {(void)x;}
static inline int mpz_init_set_str(mpz_t x, const char* s, int base) {(void)x;(void)s;(void)base; return 0;}
static inline void mpz_urandomb(mpz_t r, gmp_randstate_t state, unsigned long n) {(void)r;(void)state;(void)n;}
static inline void mpz_nextprime(mpz_t rop, const mpz_t op) {(void)rop;(void)op;}
static inline void mpz_sub_ui(mpz_t rop, const mpz_t op, unsigned long n) {(void)rop;(void)op;(void)n;}
static inline void mpz_mul(mpz_t rop, const mpz_t op1, const mpz_t op2) {(void)rop;(void)op1;(void)op2;}
static inline void mpz_gcd(mpz_t rop, const mpz_t op1, const mpz_t op2) {(void)rop;(void)op1;(void)op2;}
static inline int mpz_invert(mpz_t rop, const mpz_t op1, const mpz_t op2) {(void)rop;(void)op1;(void)op2; return 1;}
static inline void mpz_powm(mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod) {(void)rop;(void)base;(void)exp;(void)mod;}
static inline void mpz_import(mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void* op) {(void)rop;(void)count;(void)order;(void)size;(void)endian;(void)nails;(void)op;}
static inline size_t mpz_sizeinbase (const mpz_t op, int base) {(void)op;(void)base; return 0;}
static inline void mpz_export(void* rop, size_t* countp, int order, size_t size, int endian, size_t nails, const mpz_t op) {(void)rop;(void)countp;(void)order;(void)size;(void)endian;(void)nails;(void)op; *countp = 0;}
static inline void gmp_printf(const char* fmt, ...) {(void)fmt;}
static inline void gmp_randinit_default(gmp_randstate_t state) {(void)state;}
static inline void gmp_randseed_ui(gmp_randstate_t state, unsigned long seed) {(void)state;(void)seed;}
static inline int mpz_cmp_ui(const mpz_t op1, unsigned long op2) {(void)op1;(void)op2; return 0;}
static inline void mpz_add_ui(mpz_t rop, const mpz_t op1, unsigned long op2) {(void)rop;(void)op1;(void)op2;}

#endif // GMP_STUB_H
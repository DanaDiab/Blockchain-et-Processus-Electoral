#ifndef BLOCK_CHAIN_H
#define BLOCK_CHAIN_H


int naive_is_prime(long);
long modpow_naive(long a,long m,long n);
int modpow(long a, long m,long n);
int witness ( long a , long b , long d , long p );
long rand_long ( long low , long up );
int is_prime_miller ( long p , int k );
long random_prime_number(int low_size, int up_size, int k );
long extended_gcd(long s, long t, long *u, long *v);
void generate_key_value(long p, long q, long *n, long *s, long *u);
#endif

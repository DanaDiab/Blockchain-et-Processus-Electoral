#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"seance_1.h"

int naive_is_prime(long n) //Retourne 1 si n est premier et 0 sinon.
{
	for(long i=2; i<n; i++)
	{
		if(n%i==0)return 0; //Test si n est divisible par un entier compris entre 2 et n-1 inclus 
	}
	return 1;
}

long modpow_naive(long a,long m,long n){ //Retourne a^m % n
	long res=1;
	for (long i=0; i<m; i++){
		res=(res*a)%n;
	}
	return res;
}

int modpow(long a, long m,long n){	//Retourne a^m % n
	long res=a;
	while(m>1){
		if (m%2==0){	//cas où m est paire 
			res=(res*res)%n; //Elévation au carré
		}
		else{		//cas où m est impaire
			res=(res*res*a)%n; //Elévation au carré * a
		}
	m=m/2;		
	}
	return res;
}

int witness ( long a , long b , long d , long p ) { //Retourne 1 si a est temoin de Miller de p, 0 sinon.
	long x = modpow (a ,d , p ) ;
	if ( x == 1) {
		return 0;
	}
	for ( long i = 0; i < b ; i ++) {
		if ( x == p -1){
			return 0;
		}
		x = modpow (x ,2 , p ) ;
	}
	return 1;
}

long rand_long ( long low , long up ) { //Retourne un entier généré aleatoirement entre low et up
	return rand () % ( up - low +1) + low ;
}

int is_prime_miller ( long p , int k ) { //Retourne 1 si p est premier, 0 sinon. 
	if ( p == 2) {
		return 1;
	}
	if (!( p & 1) || p <= 1) { //on verifie que p est impair et different de 1
		return 0;
	}
	//on determine b et d :
	long b = 0;
	long d = p - 1;
	while (!( d & 1) ) { //tant que d n’est pas impair
		d = d /2;
		b = b +1;
	}
	// On genere k valeurs pour a, et on teste si c’est un temoin :
	long a ;
	int i ;
	for ( i = 0; i < k ; i ++) {
		a = rand_long (2 , p-1) ;
		if ( witness (a ,b ,d , p ) ) {
			return 0;
		}
	}
	return 1;
}

long random_prime_number(int low_size, int up_size, int k ){
	long min = pow(2, low_size-1); //plus petit entier codé avec low_size bits
	long max = pow(2, up_size)-1; //plus grand entier codé avec 
	long alea= rand_long(min, max);
	int premier=is_prime_miller(alea,k);
	while (premier!=1){
		alea = rand_long(min, max);
		premier=is_prime_miller(alea,k);
	}
	return alea;
}

long extended_gcd(long s, long t, long *u, long *v){
	if (t==0){
		*u=1;
		*v=0;
		return s;
	}
	long uPrim, vPrim;
	long gcd=extended_gcd(t,s%t, &uPrim,&vPrim);
	*u=vPrim;
	*v=uPrim-(s/t)*vPrim;
	return gcd;
}

void generate_key_value(long p, long q, long *n, long *s, long *u){
	*n=(p*q);
	long t=(p-1)*(q-1);
	long v=0;
	*s=rand_long(2,t-1);
	long pgcd_st=extended_gcd(*s,t,u,&v);
	while (pgcd_st!=1){
		*s=rand_long(2,t-1);
		pgcd_st=extended_gcd(*s,t,u,&v);
	}
}

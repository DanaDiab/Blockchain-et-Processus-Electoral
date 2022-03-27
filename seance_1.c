#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"seance_1.h"
#include <assert.h>

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

long modpow(long a, long m,long n){	//Retourne a^m % n
	long res=1;
	while(m>0){
		if (m%2!=0){	//cas où m est paire
			res=(res*a)%n; //Elévation au carré
		}
	a=a*a%n;
	m=m/2;
	}
	return res;
}

long modpow_rec(long a, long m, long n) //Même fonction que modpow mais de façon récusrive.
{
	long res;
	if(m==1)
	{
		return a%n;
	}
	if(m%2==0)
	{
		res=modpow_rec(a, m/2, n);
		res= (res*res)%n;
	}
	else
	{
		res=a%n;
		res=(res*modpow_rec(a, m-1, n))%n;
	}
	return res;
}

int witness( long a , long b , long d , long p ) { //Retourne 1 si a est temoin de Miller de p, 0 sinon.
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

long rand_long( long low , long up ) { //Retourne un entier généré aleatoirement entre low et up
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
	long min = powl(2, low_size-1); //plus petit entier codé avec low_size bits
	long max = powl(2, up_size)-1; //plus grand entier codé avec up_size bits.
	long alea= rand_long(min, max);
	int premier=is_prime_miller(alea,k);
	while (premier!=1){	//on gènere un nouveau entier tant qu'il n'est pas premier.
		alea = rand_long(min, max);
		premier=is_prime_miller(alea,k);
	}
	return alea;
}

long extended_gcd(long s, long t, long *u, long *v){ //Algorithme d'euclide étendu
	if (s==0){
		*u=0;
		*v=1;
		return t;
	}
	long uPrim, vPrim;
	long gcd=extended_gcd(t%s, s, &uPrim,&vPrim);
	*u=vPrim - (t/s)*uPrim;		//u et v sont les coeffiecents de Bezout vérifiant : u*s + v*t ;
	*v=uPrim;
	return gcd;			//Retourne le PGCD de s et t
}

void generate_key_values(long p, long q, long *n, long *s, long *u){ 
	*n=(p*q);
	long t=(p-1)*(q-1);
	long v=0;
	*s=rand_long(2,t-1);
	long pgcd_st=extended_gcd(*s,t,u,&v);
	while (pgcd_st!=1){ 
		*s=rand_long(2,t-1); 			//Générer un long s tel que 2 <= s < t
		pgcd_st=extended_gcd(*s,t,u,&v);	//Tant que s et t ne sont pas premiers entre eux
	}
}

long *encrypt(char *chaine, long s, long n)		//Chiffrage de chaine en un tableau de long
{
	int size = strlen(chaine);			//Retourne le longueur de chaine sans prise en compte de '\0'
	long *c = (long*)malloc(size*sizeof(long));	//Allocation dynamique du tableau de long
	int i=0;
	while (chaine[i]!='\0')
	{
		c[i]=modpow_rec((int)chaine[i], s, n);	// le long qui represente chaine[i] est (chaine[i]^s) mod n
		printf("(int)chaine[i] = %d\n", (int)chaine[i]);
		i++;
	}
	return c;	//retourne le tableau de long
}

char* decrypt(long *crypted, int size, long u, long n)
{
	char *res = (char*)malloc((size+1)*sizeof(char)); //Allocation dynamique d'un tableau de char
	for(int i=0; i<size; i++)
	{

		int mp=(int)(modpow_rec(crypted[i], u, n));	// res[i]= (cypted[i]^u) mod n 
		res[i]=(char)(mp);				//Casting en char avant de stocker le résultat
		printf("res[i] = %d\n", (int)mp);
	}
	res[size]='\0';						//Ajout de '\0' à la fin de la chaine.
	printf("res=%s\n",res);
	return res;
}

void print_long_vector ( long * result , int size )		//Affichage des tableaux de long qui répresente le chiffrage.
{
	printf ( "Vector : [" ) ;
	for(int i=0; i<size; i++)
	{
		printf("%ld \t", result[i]);
	}
 	printf ( "]\n" ) ;
 }
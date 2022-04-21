#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"seance_1.h"
#include <assert.h>

int naive_is_prime(long n)			//Retourne 1 si n est premier et 0 sinon.
{
	for(long i=2; i<n; i++)
	{
		if(n%i==0)return 0;		//Test si n est divisible par un entier compris entre 2 et n-1 inclus
	}
	return 1;
}

long modpow_naive(long a,long m,long n){	//Methode naive : Retourne a^m mod n
	long res=1;
	for (long i=0; i<m; i++){
		res=(res*a)%n;
	}
	return res;
}

long modpow(long a, long m,long n){		//Retourne a^m mod n
	long res=1;
	while(m>0){
		if (m%2!=0){			//cas où m est paire
			res=(res*a)%n;		//Elévation au carré
		}
	a=a*a%n;
	m=m/2;
	}
	return res;
}

long modpow_rec(long a, long m, long n)		//Même fonction que modpow mais de façon récusrive.
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

int witness( long a , long b , long d , long p ) {		//Retourne 1 si a est temoin de Miller de p, 0 sinon.
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

long rand_long( long low , long up ) {				//Retourne un entier généré aleatoirement entre low et up
	return rand () % ( up - low +1) + low ;
}

int is_prime_miller ( long p , int k ) {			//Retourne 1 si p est premier, 0 sinon.
	if ( p == 2) {
		return 1;
	}
	if (!( p & 1) || p <= 1) {				//on verifie que p est impair et different de 1
		return 0;
	}
	//on determine b et d :
	long b = 0;
	long d = p - 1;
	while (!( d & 1) ) {					//tant que d n’est pas impair
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

long random_prime_number(int low_size, int up_size, int k ){	//Retourne un entier premier aléatoire
	long min = powl(2, low_size-1);				//plus petit entier codé avec exactement low_size bits
	long max = powl(2, up_size)-1;				//plus grand entier codé avec exactement up_size bits.
	long alea= rand_long(min, max);				// entier aléatoir entre min et max
	int premier=is_prime_miller(alea,k);			// test s'il est premier, en répétant le test de miller k fois.
	while (premier!=1){					//gènerer un nouveau entier jusqu'à trouver un entier premier.
		alea = rand_long(min, max);
		premier=is_prime_miller(alea,k);
	}
	return alea;
}

long extended_gcd(long s, long t, long *u, long *v){		//Algorithme d'euclide étendu
	if (s==0){
		*u=0;
		*v=1;
		return t;
	}
	long uPrim, vPrim;
	long gcd=extended_gcd(t%s, s, &uPrim,&vPrim);
	*u=vPrim - (t/s)*uPrim;					//u et v sont les coeffiecents de Bezout vérifiant : u*s + v*t ;
	*v=uPrim;
	return gcd;						//Retourne le PGCD de s et t
}

void generate_key_values(long p, long q, long *n, long *s, long *u){ 
	*n=(p*q);
	long t=(p-1)*(q-1);
	long v=0;
	*s=rand_long(2,t-1);
	long pgcd_st=extended_gcd(*s,t,u,&v);		//PGCD de s et t
	while (pgcd_st!=1){ 				//Tant que s et t ne sont pas premiers entre eux
		*s=rand_long(2,t-1); 			//Générer un long s tel que 2 <= s < t
		pgcd_st=extended_gcd(*s,t,u,&v);	
	}
}

long *encrypt(char *chaine, long s, long n)		//Chiffrage de chaine en un tableau de long
{
	int size = strlen(chaine);			//Retourne le longueur de chaine sans prise en compte de '\0'
	long *c = (long*)malloc(size*sizeof(long));	//Allocation dynamique du tableau de long
	if (c==NULL){					//Erreur d'allocation
		printf("Erreur lors de l'allocation\n");
		return NULL;
	}
	int i=0;
	while (chaine[i]!='\0')
	{
		c[i]=modpow_rec((int)chaine[i], s, n);	// le long qui represente chaine[i] est (chaine[i]^s) mod n
		i++;
	}
	return c;					//retourne le tableau de long
}

char* decrypt(long *crypted, int size, long u, long n)		//Fonction qui déchiffre un tableau de long
{
	char *res = (char*)malloc((size+1)*sizeof(char)); 	//Allocation dynamique d'un tableau de char
	if (res==NULL){						//Erreur d'allocation
		printf("Erreur lors de l'allocation\n");
		return NULL;
	}
	for(int i=0; i<size; i++)
	{
		res[i]=(char)(modpow(crypted[i], u, n));	//Casting en char avant de stocker le résultat
	}
	res[size]='\0';						//Ajout de '\0' à la fin de la chaine.
	return res;
}

void print_long_vector ( long * result , int size )		//Affichage des tableaux de long qui répresente le chiffrage.
{
	printf ( "Vector : [" ) ;
	for(int i=0; i<size; i++)
	{
		printf("%ld \t", result[i]);			//Affichage d'un long
	}
 	printf ( "]\n" ) ;
 }

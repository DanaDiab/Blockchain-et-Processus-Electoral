#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"


int main ()
{
	srand(time(NULL));
	long a_test = 1234;
	long b_test = 5678;
	long u_test = 0;
	long v_test = 0;

	//Calcul du pgcd de a et b et calcul des coefficients de Bézout u et v.
	long pgcd_test = extended_gcd(a_test, b_test, &u_test, &v_test);
	printf("PGCD_test = %ld, U = %ld, V = %ld", pgcd_test, u_test, v_test);

	//Tests comparant et s'assurant des résultats des plusieurs fonctions de modpow
	assert(modpow_rec(11, 13, 19)==11);
	assert(modpow_naive(72, 2177, 5917)==1841);
	int test_mp = modpow_rec(10, 5, 85);
	printf("test_modpow : %d\n", test_mp);
	assert(modpow_rec(72, 2177, 5917)==1841);
	assert(modpow_naive(10, 5, 85)==40);
	assert(modpow_rec(10, 5, 85)==40);
	

	//Generation de cle :
	long p = random_prime_number (12 ,14 , 5000) ;
	long q = random_prime_number (12 ,14 , 5000) ;
	while ( p == q )
	{
		q = random_prime_number (12 ,14 , 5000) ;
	}
	long n , s , u ;
	generate_key_values (p ,q ,& n ,& s ,& u ) ;
	
	
	//Pour avoir des cles positives :
	if (u <0)
	{
		long t = (p -1) *( q -1);
	u = u + t ; //on aura toujours s*u mod t = 1
	}

	//Afichage des cles en hexadecimal
	printf ( "cle publique = (%ld,%ld)\n" ,s , n ) ;
	printf ( "cle privee = (%ld,%ld)\n" ,u , n ) ;

	//Chiffrement:
	char mess [256] = "Hello" ;
	int len = strlen ( mess ) ;
	long * crypted = encrypt ( mess , s , n ) ;

	printf ( "Initial message : %s \n" , mess ) ;
	printf ( " Encoded representation : \n" ) ;
	print_long_vector ( crypted , len ) ;

	//Dechiffrement
	char * decoded = decrypt ( crypted , len , u , n ) ;
	printf ( " Decoded : %s \n" , decoded ) ;	//Affiche bien 'Hello'

	free(crypted);
	free(decoded);
	return 0;
}

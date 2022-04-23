#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"


int main ()
{
	
	//CE MAIN NOUS EST DONNE DANS LE SUJET DU PROJET
	
	srand(time(NULL));

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
	printf ( " Decoded : %s \n" , decoded ) ;

	free(crypted);
	free(decoded);
	return 0;
}

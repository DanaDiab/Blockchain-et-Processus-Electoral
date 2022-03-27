#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));

	double temps_mpnaive, temps_mp;
	double temps_mpnaive_init, temps_mp_init, temps_mpnaive_fin, temps_mp_fin;


	int min = 0;
	int max = 100;
	for(int i=min; i<max; i++) //Test pour les entiers de min=0 à max=100
	{
		if(naive_is_prime(i))
		{
			printf("%d est premier !\n", i);
		}
	}


	assert(modpow_naive(4555555,80,3)==modpow(4555555,80,3)); //Test si les deux fonctions renvoient le même résultat
	assert(modpow_naive(4555555,80,3)==modpow_rec(4555555,80,3));
	printf("%ld\n", random_prime_number(5,6,10));

	long p=5, q=6, n=0, s=0,u=0;		//Test de la fonction qui genère une clé publique et secrète avec un p et q donnés 
	generate_key_values(p, q, &n, &s, &u);		
	printf("p=%ld, q=%ld, *n=%ld, *s=%ld, *u=%ld\n", p,q,n,s,u);


	FILE *fichier=fopen("sortie_vitesse_modpow.txt","w"); //Ouverture d'un fichier
	for (int i=0;i<1000000;i++){ //boucle qui calcul le temps d'execution pour chaque fonction
		temps_mpnaive_init=clock();	//temps auquel la fonction modpow_naive s'execute
		modpow_naive(3,i,5);
		temps_mpnaive_fin=clock();	//temps auquel la fonction modpow_naive se termine
		temps_mp_init=clock();
		modpow(3,i,5);
		temps_mp_fin=clock();
		temps_mpnaive=((double)(temps_mpnaive_fin-temps_mpnaive_init))/CLOCKS_PER_SEC; //Calcul tu temps en nombre de secondes.
		temps_mp= ((double)(temps_mp_fin-temps_mp_init))/CLOCKS_PER_SEC;
		fprintf(fichier,"%d %lf %lf\n", i,temps_mpnaive,temps_mp); //Ecriture dans un fichier
	}
	fclose(fichier); //fermeture du fichier
	return 0;
}

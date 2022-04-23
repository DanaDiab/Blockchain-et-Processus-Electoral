#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"

int main(int argc, char *argv[])
{
	srand(time(NULL));

	
	//Declaration
	double temps_mpnaive, temps_mp;
	double temps_mpnaive_init, temps_mp_init, temps_mpnaive_fin, temps_mp_fin;

	
	int min = 0;
	int max = 100;
	for(int i=min; i<max; i++) //Test de primalité pour les entiers de 0 à 99
	{
		if(naive_is_prime(i))
		{
			printf("%d est premier !\n", i);
		}
	}

	
	//Test si les 3 fonctions de modpow renvoient le même résultat
	assert(modpow_naive(4555555,80,3)==modpow(4555555,80,3));
	assert(modpow_naive(4555555,80,3)==modpow_rec(4555555,80,3));
	
	//Afiche un long premier codé avec entre 6 et 10 bits 
	printf("%ld\n", random_prime_number(5,6,10));

	
	//Test de la fonction qui genère les valeurs de deux clés : publique et secrète, avec un p et q donnés 
	long p=5, q=6, n=0, s=0,u=0;		
	generate_key_values(p, q, &n, &s, &u);		
	printf("p=%ld, q=%ld, *n=%ld, *s=%ld, *u=%ld\n", p,q,n,s,u);


	FILE *fichier=fopen("sortie_vitesse_modpow.txt","w"); 			//Ouverture d'un fichier
	if (fichier==NULL){							//Erreur d'ouverture
		printf("Erreur lors de l'ouverture du fichier \n");		
		return 1;
	}
	for (int i=0;i<10000;i++){ 						//boucle qui calcul le temps d'execution pour chaque fonction
		
		temps_mpnaive_init=clock();					//temps auquel la fonction modpow_naive s'execute
		modpow_naive(3,i,5);
		temps_mpnaive_fin=clock();					//temps auquel la fonction modpow_naive se termine
		
		temps_mp_init=clock();
		modpow(3,i,5);
		temps_mp_fin=clock();
		
		//Calcul du temps en secondes.
		temps_mpnaive=((double)(temps_mpnaive_fin-temps_mpnaive_init))/CLOCKS_PER_SEC;
		temps_mp= ((double)(temps_mp_fin-temps_mp_init))/CLOCKS_PER_SEC;
		
		fprintf(fichier,"%d %lf %lf\n", i,temps_mpnaive,temps_mp); 			//Ecriture dans le fichier
	}
	fclose(fichier); //fermeture du fichier
	return 0;
}

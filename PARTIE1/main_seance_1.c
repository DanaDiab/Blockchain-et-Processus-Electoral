#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"

int main(int argc, char *argv[])
{
	double temps_mpnaive, temps_mp;
	double temps_mpnaive_init, temps_mp_init, temps_mpnaive_fin, temps_mp_fin;
	int min = 0; 
	int max = 100;
	for(int i=min; i<max; i++)
	{
		if(naive_is_prime(i))
		{
			printf("%d est premier !\n", i);
		}
	}
	
	assert(modpow_naive(45,80,3)==modpow(45,80,3));
	FILE *fichier=fopen("sortie_vitesse_modpow.txt","w");
	for (int i=0;i<100000;i++){
		temps_mpnaive_init=clock();
		modpow_naive(3,i,5);
		temps_mpnaive_fin=clock();
		temps_mp_init=clock();
		modpow(3,i,5);
		temps_mp_fin=clock();
		temps_mpnaive=((double)(temps_mpnaive_fin-temps_mpnaive_init))/CLOCKS_PER_SEC;
		temps_mp= ((double)(temps_mp_fin-temps_mp_init))/CLOCKS_PER_SEC;
		fprintf(fichier,"%d %lf %lf\n", i,temps_mpnaive,temps_mp);
	}
	fclose(fichier);
	return 0;
}

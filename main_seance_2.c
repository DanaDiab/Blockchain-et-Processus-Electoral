#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"
#include "seance_2.h"

int main(){
	srand(time(NULL));
	Key* pk=(Key*)malloc(sizeof(Key));	 //variable qui sert à stocker la clé publique
	Key * sk=(Key*)malloc(sizeof(Key));  	 //variable qui sert à stocker la clé secrete

	init_pair_keys(pk,sk, 8, 10);		//fonction qui initialise une paire de clé avec un p et q aleatoire codé avec entre 8 et 10 bits
	
	
	char *cpk, *csk;
	//Teste si un print manuel des valeurs de la clé et sa serialization donne le même résultat
	cpk=key_to_str(pk);			
	printf("pk : k=%lx, n=%lx\n", pk->k, pk->n);
	printf("Serilization de pk : %s\n", cpk);
	
	csk=key_to_str(sk);
	printf("sk : k=%lx, n=%lx\n", sk->k, sk->n);
	printf("Serilization de sk : %s\n", csk);

	//Teste la fonction qui transforme un chaine de caractères en Key
	Key *pk_bis, *sk_bis;
	pk_bis = str_to_key(cpk);
	sk_bis = str_to_key(csk);
	printf("pk_bis : k=%lx, n=%lx\n", pk_bis->k, pk_bis->n);
	printf("sk_bis : k=%lx, n=%lx\n", sk_bis->k, sk_bis->n);
	
	//Fonction qui génére 20 citoyens dont 4 candidats.
	generate_random_data(20, 4);
	
	
	//Libération de la mémoire allouée par les fonctions utlisées
	free(pk_bis);
	free(sk_bis);
	free(pk);
	free(sk);
	free(csk);
	free(cpk);
	return 0;

	}

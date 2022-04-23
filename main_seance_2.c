#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"
#include "seance_2.h"

int main(){
	srand(time(NULL));
	
	//Allocation dynamique
	Key* pk=(Key*)malloc(sizeof(Key));	 //variable qui sert à stocker la clé publique
	Key * sk=(Key*)malloc(sizeof(Key));  	 //variable qui sert à stocker la clé secrete
	
	if ((sk==NULL) || (pk==NULL)){		//Erreur d'allocation 
		printf("Erreur lors de l'allocation\n");
		return 1;
	}
	init_pair_keys(pk,sk, 8, 10);		//fonction qui initialise une paire de clé avec un p et q aleatoire codé avec entre 8 et 10 bits
	
	
	char *cpk, *csk;
	Key *pk_bis, *sk_bis;
	
	//Test si un print des longs de la clé, sa serialization et deserialisation donne le même résultat		
	printf("CLE PUBLQUE\n\npk : k=%lx, n=%lx\n", pk->k, pk->n);
	
	cpk=key_to_str(pk);
	printf("Serilization de pk : %s\n", cpk);
	
	pk_bis = str_to_key(cpk);
	printf("Deserilization de pk : k=%lx, n=%lx\\nn", pk_bis->k, pk_bis->n);
	
	
	
	printf("CLE SECRETE\n\nsk : k=%lx, n=%lx\n", sk->k, sk->n);
	
	csk=key_to_str(sk);
	printf("Serilization de sk : %s\n", csk);
	
	sk_bis = str_to_key(csk);
	printf("Deserilization de sk : k=%lx, n=%lx\n\n", sk_bis->k, sk_bis->n);
	
	
	
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

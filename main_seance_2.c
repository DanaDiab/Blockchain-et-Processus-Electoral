#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"
#include "seance_2.h"

int main(){
	Key* pk=(Key*)malloc(sizeof(Key));
	Key * sk=(Key*)malloc(sizeof(Key));

	init_pair_keys(pk,sk, 8, 10);
	char *cpk, *csk;
	cpk=key_to_str(pk);
	printf("pk : k=%lx, n=%lx\n", pk->k, pk->n);
	printf("Serilization de pk : %s\n", cpk);
	csk=key_to_str(sk);
	printf("sk : k=%lx, n=%lx\n", sk->k, sk->n);
	printf("Serilization de sk : %s\n", csk);

	Key *pk_bis, *sk_bis;

	pk_bis = str_to_key(cpk);
	sk_bis = str_to_key(csk);

	printf("pk_bis : k=%lx, n=%lx\n", pk_bis->k, pk_bis->n);
	printf("sk_bis : k=%lx, n=%lx\n", sk_bis->k, sk_bis->n);
	
	generate_random_data(100, 10);
	
	free(pk_bis);
	free(sk_bis);
	free(pk);
	free(sk);
	free(csk);
	free(cpk);
	return 0;

	}

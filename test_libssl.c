#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	
	const char * s= "Rosetta code";
	unsigned char *d = SHA256((unsigned char *)s,strlen(s),0);		//Hachage de la chaine "Rosetta code" à l'aide de la fonction SHA256;
	for(int i=0; i<SHA256_DIGEST_LENGTH ;i++){				//Affichage de la valeur hachée en hexadecimal
		printf("%02x",d[i]);
	}
	putchar('\n');
	return 0;
}

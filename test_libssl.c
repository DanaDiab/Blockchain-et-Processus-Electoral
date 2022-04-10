#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	const char * s= "Rosetta code";
	unsigned char *d = SHA256((unsigned char *)s,strlen(s),0);
	for(int i=0; i<SHA256_DIGEST_LENGTH ;i++){
		printf("%02x",d[i]);
	}
	putchar('\n');
	return 0;
}

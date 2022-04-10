#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include <openssl/sha.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"
#include"seance_4.h"
#include"seance_5.h"


Block * create_block(Key * author, CellProtected* votes, unsigned char * hash, unsigned char * prev_hash, int nonce){
	Block * b=malloc(sizeof(Block));
	if (b!=NULL){
		b->author=author;
		b->votes=votes;
		b->hash=(unsigned char *) strdup((char *)hash);
		b->previous_hash=(unsigned char *) strdup((char*)prev_hash);
		b->nonce=nonce;
	}
	return b;
}


void block_to_file(Block* b, char* fichier)
{
	FILE* file = fopen(fichier, "w");
	if (file==NULL){
		printf("Erreur lors de l'ouverture du fichier\n");
		return;
	}
	if (b==NULL) return;

	char* author, * vote;	
	author = key_to_str( b->author);
	fprintf(file, "%s\n", author);
	free(author);
	CellProtected* curr = b->votes;
	fprintf(file, "%s\n", (char *)b->hash);
	fprintf(file, "%s\n", (char *) b->previous_hash);
	fprintf(file, "%d\n",b-> nonce);
	while(curr)
	{
		vote = protected_to_str(curr->data);
		fprintf(file, "%s", vote);
		free(vote);
		curr=curr->next;
	}
	fclose(file);
}


Block* file_to_block(char *fichier){
	FILE * file=fopen(fichier,"r");
	if (file==NULL){
		printf("Erreur lors de l'ouverture du fichier \n");
		return NULL;
	}

	char buffer[256];
	char author[256];
	Protected* pr;
	CellProtected* LCP=NULL;
	int nonce;
	char hash[256];
	char prev_hash[256];
	
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",author);
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",hash);
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",prev_hash);
	fgets(buffer,256,file);
	sscanf(buffer,"%d\n",&nonce);
	
	while (fgets(buffer,256,file)){
		pr=str_to_protected(buffer);
		add_cell_protected(&LCP,pr);
	}
	fclose(file);
	return create_block(str_to_key(author), LCP,(unsigned char *)hash, (unsigned char *)prev_hash,nonce);
}


char* block_to_str(Block *block)
{
	if (block==NULL)return NULL;
	int n=0;
	CellProtected* curr=block->votes;
	while(curr)
	{
		n++;
		curr=curr->next;
	}
	char* res = (char*)malloc(sizeof(char)*((n+10)*256));
	if (res==NULL) return "";
	char* author = key_to_str(block->author);
	char* vote;
	strcpy(res, author);
	strcat(res,"\n");
	strcat(res, (char *) block->previous_hash);
	strcat(res,"\n");
	curr=block->votes;
	while(curr)
	{	
		vote=protected_to_str(curr->data);
		strcat(res, vote);
		free(vote);
		curr=curr->next;
	}
	char nonce[256];
	sprintf(nonce, "%d", block->nonce);
	strcat(res, nonce);
	free(author);
	return res;
}

unsigned char * hashage(const char *s){
	return SHA256((unsigned char *)s,strlen(s), 0);
}

void affichage_hashage(unsigned char * d){
	for(int i=0; i<SHA256_DIGEST_LENGTH ;i++){
		printf("%02x",d[i]);
	}
	putchar('\n');
}


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
		b->previous_hash=(unsigned char *)strdup((char*)prev_hash);
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
	if (res==NULL) return NULL;
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
	unsigned char *hash =  SHA256((unsigned char *)s,strlen(s), 0);
	unsigned char *res=(unsigned char*)malloc(64*sizeof(unsigned char)+1);
	res[0]='\0';
	unsigned char curr[256];
	for(int i=0; i<SHA256_DIGEST_LENGTH; i++)
	{
		sprintf((char*)curr, "%02x", hash[i]);
		strcat((char*)res, (char*)curr);
	}
	return res;
}


void compute_proof_of_work(Block *b , int d){
	b->nonce=0;
	char * str_block=block_to_str(b);
	unsigned char * hash=hashage(str_block);
	int flag=0;
	while (!flag){
		for (int i=0;i<d;i++){
			if (hash[i]!='0'){ 
				flag=0;
				break;
			}
			flag=1;
		}
		if(!flag)
		{
			b->nonce++;
			free(str_block);
			free(hash);
			str_block=block_to_str(b);
			hash=hashage(str_block);
		}
	}
	free(b->hash);
	b->hash=hash;
	free(str_block);
}


int verify_block(Block *b, int d){ //return 1 si block valide;
	unsigned char *hash=b->hash;
	int flag=1;
	for (int i=0;i<d;i++){
		if (hash[i]!='0'){
			flag=0;
			break;
		}
	}
	return flag;
}
	

void delete_block(Block *b)
{
	if (b==NULL) return;
	CellProtected *parc=b->votes;
	free(b->hash);
	free(b->previous_hash);
	while(parc)
	{
		parc=parc->next;
		free(b->votes);
		b->votes=parc;
	}
	free(b);
}





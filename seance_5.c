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

//Fonction non demandée dans le sujet  : différente de la fonction demandée createBlock 
Block * create_block(Key * author, CellProtected* votes, unsigned char * hash, unsigned char * prev_hash, int nonce){
	Block * b=malloc(sizeof(Block));						//Allocation de la mémoire d'un Block
	if (b!=NULL){									//Initialisation avec les valeurs passées en paramètre
		b->author=author;
		b->votes=votes;
		b->hash=(unsigned char *) strdup((char *)hash);				//Allocation de mémoire pour copier hash
		b->previous_hash=(unsigned char *)strdup((char*)prev_hash);		//Allocation de mémoire pour copier prev_hash
		b->nonce=nonce;
	}
	return b;
}


void block_to_file(Block* b, char* fichier)					//Ecriture d'un Block dans un fichier
{
	if (b==NULL) return;							//Test si Bloc est NULL
	
	FILE* file = fopen(fichier, "w");					//Ouverture du fichier en mode ecriture
	if (file==NULL){							//Erreur d'ouverture 
		printf("Erreur lors de l'ouverture du fichier\n");
		return;
	}


	char* author, * vote;
	
	//Ecriture du Key author du Block dans le fichier
	author = key_to_str( b->author);
	fprintf(file, "%s\n", author);			
	free(author);								//Libere la mémoire allouée par la fonction de serialisation		
	

	//Ecriture du hash,previous_hash et du nonce dans le fichier
	fprintf(file, "%s\n", (char *)b->hash);
	fprintf(file, "%s\n", (char *) b->previous_hash);
	fprintf(file, "%d\n",b-> nonce);
	
	
	//Parcours de la liste de votes
	CellProtected* curr = b->votes;
	while(curr)
	{
		vote = protected_to_str(curr->data);				//Serialisation du vote
		fprintf(file, "%s", vote);					//Ecriture dans le fichier
		free(vote);							//Liberation de la mémoire allouée par la fonction de serialisation
		curr=curr->next;
	}
	fclose(file);								//Fermeture du fichier
}


Block* file_to_block(char *fichier)						//Lecture d'un Block d'un fichier
{	
	FILE * file=fopen(fichier,"r");						//Ouverture du fichier en mode lecture 
	if (file==NULL){							//Erreur d'ouverture
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
	
	//Lecture d'une ligne et extraction de la chaine de char contenant la Key author
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",author);
	
	//Lecture d'une ligne et extraction de la chaine de char contenant la valeur hachée du Block
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",hash);
	
	//Lecture d'une ligne et extraction de la chaine de char contenant la valeur hachée du Block précédent
	fgets(buffer,256,file);
	sscanf(buffer,"%s\n",prev_hash);
	
	//Lecture de l'entier nonce
	fgets(buffer,256,file);
	sscanf(buffer,"%d\n",&nonce);
	
	
	//Lecture des lignes restantes (votes) et creation d'une liste de CellProtected contenant les votes
	while (fgets(buffer,256,file)){
		pr=str_to_protected(buffer);
		add_cell_protected(&LCP,pr);
	}
	fclose(file);									//Fermeture du fichier
	
	//Creation du Block avec les valeurs lu.
	return create_block(str_to_key(author), LCP,(unsigned char *)hash, (unsigned char *)prev_hash,nonce);
}


char* block_to_str(Block *block)				//Serialisation d'un Block en chaine de char
{
	if (block==NULL)return NULL;
	
	//Compte le nb de votes dans la liste votes de Block
	int n=0;
	CellProtected* curr=block->votes;
	while(curr)
	{
		n++;
		curr=curr->next;
	}
	
	//Allocatio de la chaine de char à retourner comme résultat
	char* res = (char*)malloc(sizeof(char)*((n+10)*256));
	
	//Erreur d'allocation
	if (res==NULL) return NULL;
	
	
	char* author = key_to_str(block->author);
	char* vote;
	
	//Concatenation des champs du Block
	strcpy(res, author);					//Serialisation du Key author
	strcat(res,"\n");
	strcat(res, (char *) block->previous_hash);
	strcat(res,"\n");
	
	curr=block->votes;
	while(curr)
	{	
		vote=protected_to_str(curr->data);		//Serialisation du vote
		strcat(res, vote);
		free(vote);					//Liberation de la mémoire alloué par le fonction de serialisation
		curr=curr->next;
	}
	
	//Serialisation du nonce et sa concatenation au resultat
	char nonce[256];
	sprintf(nonce, "%d", block->nonce);			
	strcat(res, nonce);
	
	free(author);						//Liberation de la mémoire alloué par le fonction de serialisation
	
	return res;
}

unsigned char * hashage(const char *s){						//Fonction de hachage
	unsigned char *hash =  SHA256((unsigned char *)s,strlen(s), 0);		//resultat du hashage avec la fonction SHA256
	unsigned char *res=(unsigned char*)malloc(64*sizeof(unsigned char)+1);	//Allocation du resultat : ytableau de unsigned char
	res[0]='\0';
	unsigned char curr[256];
	for(int i=0; i<SHA256_DIGEST_LENGTH; i++)
	{
		sprintf((char*)curr, "%02x", hash[i]);				//Formatage de la valeur hachée en hexadecimal
		strcat((char*)res, (char*)curr);				//Concatenation les valeurs hexa dans le tableau resultat
	}
	return res;
}


void compute_proof_of_work(Block *b , int d){					//Fonction qui genere une preuve de travail
	
	b->nonce=0;								//Initialisation à 0;
	char * str_block=block_to_str(b);					//Serialisation du Block
	unsigned char * hash=hashage(str_block);				//Valeur hachée du Block
	
	int flag=0;								//Flag 1 si le nonce est bon, 0 sinon
	while (!flag){
		for (int i=0;i<d;i++){						//S'assurer que les d premiers char soient des 0
			if (hash[i]!='0'){ 
				flag=0;
				break;
			}
			flag=1;
		}
		if(!flag)							//Si un des d premiers char est different de 0
		{
			b->nonce++;						//Incrementer nonce
			
			free(str_block);					//Liberer la memoire allouée
			free(hash);
			
			str_block=block_to_str(b);				//Serialisation avec la nouvelle valeur du nonce
			hash=hashage(str_block);				//Nouveau hashage
		}
	}
	free(b->hash);								//Liberation de l'ancienne valeur hashée stocké dans le Block
	b->hash=hash;								//Affectation de la nouvelle valeur hachée
	free(str_block);							//Liberation de la memoire alloué par la fonction de serialisation
}


int verify_block(Block *b, int d){						//Return 1 si block valide, 0 sinon
	unsigned char *hash=b->hash;
	int flag=1;
	for (int i=0;i<d;i++){							//Comparaison les d premiers char à 0
		if (hash[i]!='0'){						
			flag=0;
			break;
		}
	}
	return flag;
}
	

void delete_block(Block *b)							//Liberation de la mémoire occupé par un Block
{
	if (b==NULL) return;
	CellProtected *parc=b->votes;
	
	//Free des valeurs hachées
	free(b->hash);								
	free(b->previous_hash);

	//Free des CellProtected mais pas de Protected eux-mêmes
	while(parc)
	{
		parc=parc->next;			
		free(b->votes);
		b->votes=parc;
	}	
	//Free du Block
	free(b);
}





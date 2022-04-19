#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include <openssl/sha.h>
#include <dirent.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"
#include"seance_4.h"
#include"seance_5.h"
#include"seance_6.h"
#include"seance_7.h"

void submit_vote(Protected *p){
	FILE * file=fopen("Pending_votes.txt","a");
	if (file==NULL){
		printf("Erreur lors de l'ouverture du fichier\n");
		return;
	}
	char * str= protected_to_str(p);
	fprintf(file,"%s",str);
	free(str);
	fclose(file);
	return;
}

void createBlock(CellTree* tree,Key* author, int d){
	CellProtected *LCP=read_protected("Pending_votes.txt");
	char hash[10]="hash";
	char prev_hash[10]=".";
	CellTree* last=last_node(tree);
	Block*b=NULL;
	if (last==NULL){
		b=create_block(author, LCP,(unsigned char *)hash,(unsigned char *)prev_hash,0);
	}
	else{
		b=create_block(author, LCP,(unsigned char *)hash,last->block->hash,0);
	}
	compute_proof_of_work(b,d);
	int del=remove("Pending_votes.txt");
	if (del==0) printf("Le fichier Pending_votes.txt a été supprimé.\n");
	else printf("Le fichier Pending_votes.txt n'a PAS été supprimé.\n");
	block_to_file(b,"Pending_block.txt");

	//temporaire (comprendre le role de tree)
	b->votes=NULL;
	delete_list_protected(LCP);
	delete_block(b);
	return;
}

void add_block(int d,char * name){
	Block * block=file_to_block("Pending_block.txt");
	if (verify_block(block,d)!=1){
		printf("Block non valide\n");
		int del=remove("Pending_block.txt");
		if (del==0) printf("Le fichier Pending_block.txt a été supprimé.\n");
		else printf("Le fichier Pending_block.txt n'a PAS été supprimé.\n");
		return;
	}
	char * str=block_to_str(block);
	char dir[20]="Blockchain/";
	strcat(dir,name);
	block_to_file(block,dir);
	int del=remove("Pending_block.txt");
	if (del==0) printf("Le fichier Pending_block.txt a été supprimé.\n");
	else printf("Le fichier Pending_block.txt n'a PAS été supprimé.\n");
	free(str);
	free(block->author);
	delete_list_protected(block->votes);
	block->votes=NULL;
	delete_block(block);
	return;
}

CellTree* read_tree(){
	DIR * rep_count =opendir("./Blockchain/");
	if (rep_count==NULL) return NULL;
	Block * b =NULL;
	int size=0;
	struct dirent * dir;
	while ((dir = readdir(rep_count))){
		if ((strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name,"..")!=0)){
			size++;
		}
	}
	closedir(rep_count);
	DIR * rep =opendir("./Blockchain/");
	if (rep==NULL) return NULL;
	CellTree** T=malloc(sizeof(CellTree*)*size);
	if (T!=NULL){
		for (int i=0;i<size;i++){
			T[i]=NULL;
		}
		int i=0;
		if (rep!=NULL){
			struct dirent * dir;
			while ((dir = readdir(rep))){
				if ((strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name,"..")!=0)){
					char chemin[30]="Blockchain/";
					strcat(chemin,dir->d_name);
					b =file_to_block(chemin);
					if (b!=NULL){
						T[i]=create_node(b);
						if (T[i]!=NULL) i++;
					}
				}
			}
		}
		closedir(rep);
		for (int i=0;i<size;i++){
			for (int j=0;j<size;j++){
				if ((T[i]!=NULL) && (T[j]!=NULL)){
					if (strcmp((char*)T[i]->block->hash,(char*)T[j]->block->previous_hash)==0){
						add_child(&T[i],T[j]);
					}
				}
			}
		}
		for (int i=0;i<size;i++){
			if ((T[i]!=NULL) && (T[i]->father==NULL)){
				CellTree* racine=T[i];
				free(T);
				return racine;
			}
		}
	}
	return NULL;
}
				
			
Key * compute_winner_BT(CellTree* tree, CellKey * candidates, CellKey* voters,int sizeC, int sizeV){
	if (tree==NULL) return NULL;
	CellProtected* LCP=fusion_highest_child(tree);
	delete_non_valide(&LCP);
	Key * key=compute_winner(LCP,candidates,voters,sizeC,sizeV);
	return key;
}

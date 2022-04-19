#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"

CellKey* create_cell_key(Key* key){		//Creation d'une cellule de la liste
	CellKey * cell=malloc(sizeof(CellKey));	//Allocation d'une cellule CellKey
	if (cell!=NULL){
		cell->data=key;			//Initialisation des champs de CellKey
		cell->next=NULL;
	}
	return cell;
}

void add_cell_key(CellKey** cell, Key * key){	//Creation d'une nouvelle cellule et l'ajouter dans la liste.
	CellKey* ck=create_cell_key(key);
	if (ck!=NULL){
		ck->next=*cell;			//Ajout en tête de liste
		*cell=ck;
	}
}

CellKey* read_public_keys(char *fichier){
	CellKey * LCK=NULL;	//Declaration de la liste et l'initialisation à NULL;
	if ((strcmp(fichier,"keys.txt")==0) || (strcmp(fichier,"candidates.txt")==0)){
		FILE *f=fopen(fichier,"r");
		char line[256];
		char pkey[256];
		char skey[256];
		Key * key;
		if (f==NULL){
			printf("Erreur lors de l'ouverture du fichier \n");
			return NULL;
		}
		while (fgets(line,256,f)){			//Lecture des lignes du fichier
			sscanf(line,"%s %s\n",pkey,skey); 	//Extraire les clés publiques et secretes
			key=str_to_key(pkey);
			add_cell_key(&LCK,key);			// Ajout d'une nouvelle cellule contenant la clé publique	
		}
		fclose(f);
	}
	return LCK;
}
		
void print_list_keys(CellKey* LCK){
	char* key;
	while (LCK){
		key=key_to_str(LCK->data);
		printf("%s\n", key);	//Affichage des clés dans la liste
		LCK=LCK->next;
		free(key);
	}
}

void delete_cell_key(CellKey* c){	//Suppression d'une cellule de la liste
	free(c->data);
	free(c);
}

void delete_list_keys(CellKey * LCK){	//Suppression de toutes les cellules de la liste
	CellKey * cell=NULL;
	while (LCK){
		cell=LCK;
		LCK=LCK->next;
		delete_cell_key(cell);
	}
}

CellProtected* create_cell_protected(Protected *pr){		//Creation d'une CellProtected
	CellProtected * cell=malloc(sizeof(CellProtected));	//Allocation
	if (cell!=NULL){
		cell->data=pr;					//Initialistion
		cell->next=NULL;
	}
	return cell;
}

void add_cell_protected(CellProtected **LCP, Protected * pr){	//Ajout d'une nouvelle cellule à la liste
	CellProtected * cell=create_cell_protected(pr);		//Creation d'une nouvelle cellule
	if (cell!=NULL){
		cell->next=*LCP;				//Ajout en tête de liste
		*LCP=cell;
	}
}

CellProtected* read_protected(char *fic){		//Creation d'une liste CellProtected à partir d'un fichier
	FILE* f=fopen(fic,"r");	//ouverture du fichier
	CellProtected* LCP=NULL;
	if (f==NULL){
		printf("Erreur lors de l'ouverture du fichier\n");
		return NULL;
	}
	char line[512];		
	while (fgets(line,512,f)){	//lecture des lignes
		Protected *p=str_to_protected(line);	//Creation du protected
		add_cell_protected(&LCP,p);		//Ajout de la nouvelle cellule en tête de liste
	}
	fclose(f);	
	return LCP;
}
		
void print_list_protected(CellProtected* LCP){
	char *pr=NULL;
	while (LCP){
		pr=protected_to_str(LCP->data);
		printf("%s\n", pr);			//Affichage des Protected dans les cellules de la liste
		LCP=LCP->next;
		free(pr);
	}
}

void delete_cell_protected(CellProtected *c){		//Liberation de la memoire d'une CellProtected
	free(c->data->mess);
	free(c->data->pkey);
	free(c->data->sign->content);
	free(c->data->sign);
	free(c->data);
	free(c);
}

void delete_list_protected(CellProtected * LCP){	//Liberation de la memoire de toute la liste
	CellProtected* cell=NULL;
	while (LCP){
		cell=LCP;
		LCP=LCP->next;
		delete_cell_protected(cell);
	}
}

void delete_non_valide(CellProtected ** LCP){
	CellProtected * prec=NULL;
	CellProtected * curr=*LCP;
	CellProtected * temp=NULL;
	while (curr!=NULL){
		if (verify(curr->data)==0){			//Cas où la signature n'est pas valide
			if (prec!=NULL){
				temp=curr;
				curr=curr->next;
				prec->next=curr;
				delete_cell_protected(temp);	//Libération d'une cellule
					
			}
			else{
				*LCP=curr->next;
				temp=curr;
				curr=curr->next;
				delete_cell_protected(temp);
				
			}
		}
		else{						//Cas ou la signature est valide
			prec=curr;
			curr=curr->next;
		}
	}
}

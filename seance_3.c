#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"

CellKey* create_cell_key(Key* key){				//Creation d'une cellule de la liste
	CellKey * cell=malloc(sizeof(CellKey));			//Allocation d'une cellule CellKey
	if (cell!=NULL){					//Si l'allocation réussi
		cell->data=key;					//Initialisation des champs de CellKey
		cell->next=NULL;
	}
	return cell;
}

void add_cell_key(CellKey** cell, Key * key){			
	CellKey* ck=create_cell_key(key);			//Creation d'une CellKey
	if (ck!=NULL){
		ck->next=*cell;					//Ajout en tête de liste
		*cell=ck;
	}
}

CellKey* read_public_keys(char *fichier){			//Creation d'une liste de CellKey à partir d'un fichier
	CellKey * LCK=NULL;					//Declaration de la liste de CellKey 
	
	//Fichier doit avoir pour nom "keys.txt" ou "candidates.txt"
	if ((strcmp(fichier,"keys.txt")==0) || (strcmp(fichier,"candidates.txt")==0)){
		FILE *f=fopen(fichier,"r");			//Ouverture du fichier en mode lecture
		
		char line[256];					//Variable stockant les lignes du fichier
		char pkey[256];
		char skey[256];
		Key * key;
		
		if (f==NULL){					//Erreur d'ouverture
			printf("Erreur lors de l'ouverture du fichier \n");
			return NULL;
		}
		while (fgets(line,256,f)){			//Lecture du fichier en stockant une ligne à la fois dans line
			sscanf(line,"%s %s\n",pkey,skey); 	//Extraire la clé publique et secrete de line
			key=str_to_key(pkey);			//Deserialistion de la clé publique
			add_cell_key(&LCK,key);			// Ajout d'une nouvelle cellule dans la liste contenant la clé publique	
		}
		fclose(f);					//Fermeture de fichier
	}
	return LCK;
}
		
void print_list_keys(CellKey* LCK){
	char* key;
	while (LCK){
		key=key_to_str(LCK->data);		//Serialisation des Key contenus dans les CellKey
		printf("%s\n", key);			//Affichage des clés dans la liste
		LCK=LCK->next;
		free(key);				//Liberation de la mémoire allouée par la fonction de serialisation
	}
}

void delete_cell_key(CellKey* c){	//Suppression d'une cellule de la liste
	free(c->data);			//Suppression de la Key
	free(c);			//Suppression de la Cellule
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
	CellProtected * cell=malloc(sizeof(CellProtected));	//Allocation d'une CellProtected
	if (cell!=NULL){
		cell->data=pr;					//Initialisation
		cell->next=NULL;
	}
	return cell;
}

void add_cell_protected(CellProtected **LCP, Protected * pr){	//Ajout d'une nouvelle cellule à la liste
	CellProtected * cell=create_cell_protected(pr);		//Creation d'une nouvelle CellProtected
	if (cell!=NULL){					//Ajout en tête de liste
		cell->next=*LCP;				
		*LCP=cell;
	}
}

CellProtected* read_protected(char *fic){		//Creation d'une liste CellProtected à partir d'un fichier
	FILE* f=fopen(fic,"r");				//Ouverture du fichier en mode lecture
	CellProtected* LCP=NULL;			//Declaration de la liste de CellProtected
	if (f==NULL){					//Erreur d'ouverture
		printf("Erreur lors de l'ouverture du fichier\n");
		return NULL;
	}
	char line[512];		
	while (fgets(line,512,f)){			//lecture des lignes, et stockage une par une dans line
		Protected *p=str_to_protected(line);	//Creation du protected
		add_cell_protected(&LCP,p);		//Ajout de la nouvelle cellule en tête de liste
	}
	fclose(f);					//Fermeture du fichier
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
	//Liberation de la memoire du Protected dans la cellule
	free(c->data->mess);
	free(c->data->pkey);
	free(c->data->sign->content);
	free(c->data->sign);
	free(c->data);
	//Liberation de CellProtected
	free(c);
}

void delete_list_protected(CellProtected * LCP){	//Liberation de la memoire de toute la liste CellProtected
	CellProtected* cell=NULL;			//variable temporaire
	while (LCP){
		cell=LCP;
		LCP=LCP->next;
		delete_cell_protected(cell);		//Suppression d'une cellule
	}
}

void delete_non_valide(CellProtected ** LCP){
	CellProtected * prec=NULL;
	CellProtected * curr=*LCP;
	CellProtected * temp=NULL;
	while (curr!=NULL){
		if (verify(curr->data)==0){			//Cas où la signature n'est pas valide
			if (prec!=NULL){			//Cellule à supprimer pas en tete de liste
				temp=curr;
				
				//Chainage entre la cellule qui est avant et celle qui est après la cellule à supprimer
				curr=curr->next;
				prec->next=curr;
				
				delete_cell_protected(temp);	//Libération d'une cellule
					
			}
			else{					//Cellule à supprimer est en tête de liste
				*LCP=curr->next;		//Changement du pointeur qui pointe vers la tete de la liste
				temp=curr;
				curr=curr->next;
				delete_cell_protected(temp);	//Libération d'une cellule
				
			}
		}
		else{						//Cas ou la signature est valide
			prec=curr;
			curr=curr->next;
		}
	}
}

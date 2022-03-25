#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"

CellKey* create_cell_key(Key* key){
	CellKey * cell=malloc(sizeof(CellKey));
	if (cell!=NULL){
		cell->data=key;
		cell->next=NULL;
	}
	return cell;
}

void add_cell_key(CellKey** cell, Key * key){
	CellKey* ck=create_cell_key(key);
	if (ck!=NULL){
		ck->next=*cell;
		*cell=ck;
	}
}

CellKey* read_public_keys(char *fichier){
	CellKey * LCK=NULL;
	if ((strcmp(fichier,"keys.txt")==0) || (strcmp(fichier,"candidates.txt")==0)){
		FILE *f=fopen(fichier,"r");
		char line[256];
		char pkey[256];
		char skey[256];
		Key * key;
		if (f!=NULL){
			while (fgets(line,256,f)){
				sscanf(line,"%s %s\n",pkey,skey); 
				key=str_to_key(pkey);
				add_cell_key(&LCK,key);
			}
			fclose(f);
		}
	}
	return LCK;
}
		
void print_list_keys(CellKey* LCK){
	char* key;
	while (LCK){
		key=key_to_str(LCK->data);
		printf("%s\n", key);
		LCK=LCK->next;
		free(key);
	}
}

void delete_cell_key(CellKey* c){
	free(c->data);
	free(c);
}

void delete_list_keys(CellKey * LCK){
	CellKey * cell=NULL;
	while (LCK){
		cell=LCK;
		LCK=LCK->next;
		delete_cell_key(cell);
	}
}

CellProtected* create_cell_protected(Protected *pr){
	CellProtected * cell=malloc(sizeof(CellProtected));
	if (cell!=NULL){
		cell->data=pr;
		cell->next=NULL;
	}
	return cell;
}

void add_cell_protected(CellProtected **LCP, Protected * pr){
	CellProtected * cell=create_cell_protected(pr);
	if (cell!=NULL){
		cell->next=*LCP;
		*LCP=cell;
	}
}

CellProtected* read_protected(){
	FILE* f=fopen("declarations.txt","r");
	CellProtected* LCP=NULL;
	if (f!=NULL){
		char line[256];
		while (fgets(line,256,f)){
			Protected *p=str_to_protected(line);
			add_cell_protected(&LCP,p);
		}
		fclose(f);
	}	
	return LCP;
}
		
void print_list_protected(CellProtected* LCP){
	char *pr=NULL;
	while (LCP){
		pr=protected_to_str(LCP->data);
		printf("%s\n", pr);
		LCP=LCP->next;
		free(pr);
	}
}

void delete_cell_protected(CellProtected *c){
	free(c->data->mess);
	free(c->data->pkey);
	free(c->data->sign->content);
	free(c->data->sign);
	free(c->data);
	free(c);
}

void delete_list_protected(CellProtected * LCP){
	CellProtected* cell=NULL;
	while (LCP){
		cell=LCP;
		LCP=LCP->next;
		delete_cell_protected(cell);
	}
}

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"
#include"seance_4.h"

HashCell *create_hashcell(Key* key)
{
	HashCell*new=malloc(sizeof(HashCell));
	if (new!=NULL){
		Key* new_key=malloc(sizeof(Key));
		if (new_key!=NULL){
			new_key->k=key->k;
			new_key->n=key->n;
		}
		new->key=new_key;
		new->val=0;
	}
	return new;
}

int hash_function(Key* key, int size)
{
	if (key==NULL) return -1;
	return ((key->k)+(key->n))%size;
}

int find_position(HashTable* t, Key* key)
{	
	if(key != NULL)
	{
		int index= hash_function(key, t->size);
		//printf("INDEX : %d\n", index);
		int hash_index=index; //Si on doit procéder à un probing on garde la valeur initiale de l'index.
		do
		{
			if(t->tab[index]==NULL || (((((t->tab[index])->key)->k)==(key->k)) && ((((t->tab[index])->key)->n)==(key->n))) )return index;
			index=(index+1)%t->size; //si l'emplacement n'est pas disponible ou qu'il ne contient pas deja la valeur cherchée, on teste la case suivante pour le probing
		}
		while(index%(t->size)!=hash_index);
	}
	return -1; //si il est impossible d'insérer alors on retourne -1
}

HashTable * create_hashtable(CellKey* keys, int size){
		HashTable * hashtab=(HashTable*)malloc(sizeof(HashTable));
		if (hashtab==NULL) return NULL;
		hashtab->size=size;
		hashtab->tab=(HashCell**)malloc(sizeof(HashCell*)*size);
		//on initialise tout a NULL pour eviter les segfaults...
		for(int i=0; i<size; i++)
		{
			hashtab->tab[i]=NULL;
		}	
		int index=-1;
		while(keys){
			index=find_position(hashtab,keys->data);
			if (index!=-1){
				hashtab->tab[index]=create_hashcell(keys->data);
			}
			else{
				printf("La table de hachage est pleine . \n");
				break;
			}
			keys=keys->next;
		}
		return hashtab;
}


void delete_hashtable(HashTable* t)
{
	for(int i=0; i<t->size; i++)
	{
		if (t->tab[i]!=NULL){
			free(t->tab[i]->key);
		}
		free(t->tab[i]);
	}
	free(t->tab);
	free(t);
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV)
{
	HashTable* Hc = create_hashtable(candidates, sizeC);
	HashTable* Hv = create_hashtable(voters, sizeV);
	
	int pos_v,pos_c;
	while (decl){
		pos_v=-1;
		pos_v=find_position(Hv,decl->data->pkey);
		if ((pos_v!=-1) && (Hv->tab[pos_v]!=NULL)){
			if (Hv->tab[pos_v]->val==0){
				Key * candidat=str_to_key(decl->data->mess);
				pos_c=-1;
				pos_c=find_position(Hc,candidat);
				if ((pos_c!=-1) && (Hc->tab[pos_c]!=NULL)){
					Hc->tab[pos_c]->val++;
					Hv->tab[pos_v]->val=1;
				}
				free(candidat);
			}
		}
		decl=decl->next;
	}
	Key * max=NULL;
	int val=-1;
	for (int i=0; i<sizeC; i++){
		if ((Hc->tab[i]!=NULL) && (Hc->tab[i]->val>val)){
			max=Hc->tab[i]->key;
			val=Hc->tab[i]->val;
		}
	}
	Key * res=malloc(sizeof(Key));
	if ((res!=NULL) && (max!=NULL)){
		res->k=max->k;
		res->n=max->n;
	}
	delete_hashtable(Hv);
	delete_hashtable(Hc);
	return res;	
}

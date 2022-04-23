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

HashCell *create_hashcell(Key* key)				//Alloue et initialise une HashCell
{
	HashCell* new=malloc(sizeof(HashCell));			//Allocation de la cellule
	if (new!=NULL){						
		Key* new_key=malloc(sizeof(Key));		//Allocation d'une Key que doit contenir la cellule
		if (new_key!=NULL){
			new_key->k=key->k;			//Initialisation avec les valeurs de la Key passée en paramtère
			new_key->n=key->n;
		}
		new->key=new_key;				//Initalisation de la cellule
		new->val=0;
	}
	return new;
}

int hash_function(Key* key, int size)				//Fonction de hachage
{
	if (key==NULL) return -1;
	return ((key->k)+(key->n))%size;			//Entier renvoyer entre 0 et size-1  avec size la taille de la table de hachage
}

int find_position(HashTable* t, Key* key)
{	
	if(key != NULL)
	{
		int index= hash_function(key, t->size);		//Index de la table où la key passée en paramtere doit être inserer
		int hash_index=index; 				//Si on doit procéder à un probing on garde la valeur initiale de l'index.
		do
		{
			//Cas où la case est vide ou contient une autre key avec les memes valeurs (doublons)
			if ((t->tab[index]==NULL) || (((((t->tab[index])->key)->k)==(key->k)) && ((((t->tab[index])->key)->n)==(key->n)))){
				return index;
			}
			
			index=(index+1)%t->size; 		//si l'emplacement n'est pas disponible ou qu'il ne contient pas deja la valeur cherchée, on teste la case suivante pour le probing
		}		
		while(index%(t->size)!=hash_index);		//Jusqu'à retomber sur l'index intiale
	}
	return -1; //si il est impossible d'insérer alors on retourne -1
}

HashTable * create_hashtable(CellKey* keys, int size){				//Creation d'une HashTable
		HashTable * hashtab=(HashTable*)malloc(sizeof(HashTable));	//Allocation de la HashTable
		if (hashtab==NULL){						//Erreur d'allocation
			printf("Erreur lors de l'allocation\n");
			return NULL;
		}
	
		hashtab->size=size;						
		hashtab->tab=(HashCell**)malloc(sizeof(HashCell*)*size);	//Allocation de la table de HashCell de taille size
		if (hashtab->tab==NULL){					//Erreur d'allocation
			printf("Erreur lors de l'allocation\n");
			return NULL;
		}
	
		//on initialise tout a NULL pour eviter les segfaults...
		for(int i=0; i<size; i++)
		{
			hashtab->tab[i]=NULL;
		}	
	
	
		int index=-1;
		while(keys){							//Pour chaque CellKey
			index=find_position(hashtab,keys->data);		//Trouver l'index où il faut inserer
			if (index!=-1){						//Possibilité d'insertion
				
				//La case est vide et ne contient donc pas un doublons qu'on risque d'écraser
				if (hashtab->tab[index]==NULL){			 
					hashtab->tab[index]=create_hashcell(keys->data);	//Insertion d'une nouvelle HashCell
				}
			}
			else{
				break;
			}
			keys=keys->next;
		}
	return hashtab;
}


void delete_hashtable(HashTable* t)			//Liberation de la memoire d'une HashTable
{
	for(int i=0; i<t->size; i++)
	{
		if (t->tab[i]!=NULL){
			free(t->tab[i]->key);		//Liberation du Key
		}
		free(t->tab[i]); 			//Liberation d'une case de la table de hachage
			
	}
	free(t->tab);					//Liberation de la table de hachage
	free(t);					//liberation de la HashTable
}


Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV)		//Retourne la clé publique du candidat gagnant de l'éléction
{
	if ((decl==NULL) || (candidates==NULL) || (voters==NULL))return NULL;					//Si une des listes est vide, pas de gagnant
	
	//Creation des HashTable avec leurs tailles respectives et initialisation avec les Keys des votants et des candidats: 
	HashTable* Hc = create_hashtable(candidates, sizeC);			//Gère le nombre de vote par candidat
	HashTable* Hv = create_hashtable(voters, sizeV);			//Gère le fait que chaque votant a le droit de voter et qu'il ne l'a pas déjà fait
	
	//PARTIE I : COMPTE DE VOTES
	
	int pos_v,pos_c;
	
	while (decl){								//Parcourir la liste des votes
		pos_v=-1;
		pos_v=find_position(Hv,decl->data->pkey);			//Position du voter dans table de hashage
		
		if ((pos_v!=-1) && (Hv->tab[pos_v]!=NULL)){			//S'assurer qu'il existe dans la table (a le droit de voter)
			if (Hv->tab[pos_v]->val==0){				//S'assurer qu'il n 'a pas déjà voter
				
				Key * candidat=str_to_key(decl->data->mess);	//Key du candidat à qui il a voté à partir du message contenu dans le Protected
				
				pos_c=-1;
				pos_c=find_position(Hc,candidat);		//Position du candidat dans la table de hachage
				
				if ((pos_c!=-1) && (Hc->tab[pos_c]!=NULL)){	//S'assurer que c'est vraiment un candidat aux elections
					Hc->tab[pos_c]->val++;			//Incrementer le nombre de vote reçu
					Hv->tab[pos_v]->val=1;			//Signaler que le votant à bien voter
				}
				free(candidat);					//Liberer la memoire alloué par la fonction str_to_key
			}
		}
		decl=decl->next;
	}
	
	//PARTIE II : RECHERCHE DU GAGNANT
	
	Key * max=NULL;
	int val=-1;
	
	//Recherche le candidat qui a eu le plus grand nb de votes
	for (int i=0; i<sizeC; i++){
		if ((Hc->tab[i]!=NULL) && (Hc->tab[i]->val>val)){
			max=Hc->tab[i]->key;
			val=Hc->tab[i]->val;
		}
	}
	
	//Allocation d'une Key à retourner, qui va stocker les valeurs de la Key du gagnant
	Key * res=malloc(sizeof(Key));
	//Initialisation de la Key
	if ((res!=NULL) && (max!=NULL)){
		res->k=max->k;
		res->n=max->n;
	}
	
	//Liberation de la mémoire allouée par les HashTable
	delete_hashtable(Hv);
	delete_hashtable(Hc);
	return res;	
}

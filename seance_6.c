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
#include"seance_6.h"

CellTree* create_node(Block *b){				//Creation d'un noeud de l'arbre
	CellTree* tree=(CellTree*)malloc(sizeof(CellTree));	//Allocation d'un Node
	if (tree!=NULL){					//Initialisation
		tree->block=b;
		tree->father=NULL;
		tree->firstChild=NULL;
		tree->nextBro=NULL;
		tree->height=0;					//Arbre à un noeud a pour hauteur 0
	}
	return tree;
}

int update_height(CellTree* father,CellTree* child){		//Retourne 1 si la mise à jour de l'hauteur de father à eu lieur, 0 sinon
	if (father->height<child->height+1){			
		father->height=child->height+1;
		return 1;
	}
	return 0;
}

void add_child(CellTree** father, CellTree* child)	//Ajout de child dans l'arbre father
{	
	if((child==NULL)){
		return;					//Cas 1 : Pas d'ajout, father reste intacte
	}			
	
	if(*father==NULL){
		*father=child;				//Cas 2 : Si father est nul, l'arbre conteitn un noeud: child
		return;			
	}
	
	child->father=(*father);
	
	//Ajout de child en tete de liste des fils de father
	child->nextBro=(*father)->firstChild;
	(*father)->firstChild=child;
	
	//mise a jour des ascendants :
	CellTree *curr=child->father; 		//curr est le pere actuel
	CellTree *last=child;			//last est le dernier fils
	while(curr)
	{
		update_height(curr, last);	//Mise à jour des hauteurs
		last=curr;
		curr=curr->father;
	}
}

void affiche_noeud(CellTree *t){		//Affichage d'un seul noeud
	
	printf("________________________________________________________________________\n\n");
	printf("HAUTEUR : %d\n", t->height);			//Affichage de l'hauteur
	printf("NOM : %s\n", t->block->hash);			//Affichage de la valeur haché
	if(t->father!=NULL)
	{
		printf("PERE : %s\n", t->father->block->hash);	//Affichage de la valeur haché du père
	
	}
	else
	{
		printf("RACINE\n");				//Si le noeud n'a pas de père, c'est qu'il est la racine
	}
	printf("________________________________________________________________________\n");
	return;
}

void print_tree(CellTree *t)					//Fonction récursive d'affichage d'un arbre
{
	if (t==NULL)						//Cas de Base
	{
		return;
	}
	affiche_noeud(t);					//Affichage du noeud actuel
	print_tree(t->firstChild);				//Appel recursive avec les fils du noeud actuel
	print_tree(t->nextBro);					//Appel recursive avec les freres du noeud actuel
	return;
}

void delete_node(CellTree *node)				//Suppression d'un noeud
{
	delete_block(node->block);				//Suppression du Block contenu dans le noeud
	free(node);						//Suprresion du noeud
}

void delete_tree(CellTree *tree)				//Fonction recursive de suppression d'un arbre
{
	if(tree==NULL)						///Cas de Base
	{
		return;
	}
	delete_tree(tree->firstChild);				//Suppression des fils
	delete_tree(tree->nextBro);				//Suppression des freres
	delete_node(tree);					//Suppression du noeud
	return;
}

CellTree* highest_child(CellTree* cell){				//Retourne le fils avec la plus grande hauteur
	if ((cell->firstChild==NULL) || (cell==NULL)) return NULL;	//Arbre nulle ou sans fils
	
	CellTree* child= cell->firstChild->nextBro;			//Deuxième fils	du noeud cell
	CellTree* max=cell->firstChild;					//Premier fils du noeud cell
	
	//Recherche du maximum
	while (child!=NULL){
		if (child->height>max->height){
			max=child;
		}
		child=child->nextBro;
	}
	return max;
}

CellTree* last_node(CellTree* tree){					//Fonction recursive : Retourne le dernier noeud de la plus chaine des fils de tree
	if ((tree==NULL) || (tree->firstChild==NULL)) return tree;	//Cas de Base: Pas de noeud ou arbre à un noeud
	return last_node(highest_child(tree));				//Appel recursive
}

void fusion_cell_protected(CellProtected ** fst, CellProtected ** snd){
//Complexité est linéaire ( omega (taille de fst));
//liste doublement chainée => complexité O(1)
	if (*fst==NULL){			//Si la liste fst est NULL, on pointe direct vers la tête de la liste snd
		*fst=*snd;
		return;
	}
	if (*snd==NULL) return;			//Si la liste snd est NULL, on change rien
	
	CellProtected * tmp= *fst;
	while (tmp->next!=NULL){		//Iterer la liste jusqu'à arriver au dernier élément
		tmp=tmp->next;
	}
	tmp->next=*snd;				//Chainage entre le dernier élément de fst et le premier élément de snd
}


CellProtected* fusion_highest_child(CellTree * tree){
	CellTree* curr=tree;
	CellProtected *LCP=NULL;					//Liste resultat
	while (curr!=NULL){	
		fusion_cell_protected(&LCP, &(curr->block->votes));	//fusion de la liste resultat avec la liste des votes du noeud actuel
		curr=highest_child(curr);				//mise à jour du noeud acutel : Noeud ayant la plus grande hauteur
	}
	return LCP;
}



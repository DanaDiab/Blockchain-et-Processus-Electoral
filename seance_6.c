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

CellTree* create_node(Block *b){
	CellTree* tree=(CellTree*)malloc(sizeof(CellTree));
	if (tree!=NULL){
		tree->block=b;
		tree->father=NULL;
		tree->firstChild=NULL;
		tree->nextBro=NULL;
		tree->height=0;
	}
	return tree;
}

int update_height(CellTree* father,CellTree* child){
	if (father->height<child->height+1){
		father->height=child->height+1;
		return 1;
	}
	return 0;
}

void add_child(CellTree** father, CellTree* child)
{	
	if((child==NULL)){return;}
	if(*father==NULL){*father=child; return;}
	child->father=(*father);
	child->nextBro=(*father)->firstChild;
	(*father)->firstChild=child;
	//mise a jour des ascendants :
	CellTree *curr=child->father; //curr est le pere actuel
	CellTree *last=child;			//last est le dernier fils
	while(curr)
	{
		update_height(curr, last);
		last=curr;
		curr=curr->father;
	}
}

void affiche_noeud(CellTree *t){	printf("________________________________________________________________________\n\n");
	printf("HAUTEUR : %d\n", t->height);
	printf("NOM : %s\n", t->block->hash);
	if(t->father!=NULL)
	{
		printf("PERE : %s\n", t->father->block->hash);
	
	}
	else
	{
		printf("RACINE\n");
	}
	printf("________________________________________________________________________\n");
	return;
}
void print_tree(CellTree *t)
{
	if (t==NULL)
	{
		return;
	}
	affiche_noeud(t);
	print_tree(t->firstChild);
	print_tree(t->nextBro);
	return;
}

void delete_node(CellTree *node)
{
	delete_block(node->block);
	free(node);
}

void delete_tree(CellTree *tree)
{
	if(tree==NULL)
	{
		return;
	}
	delete_tree(tree->firstChild);
	delete_tree(tree->nextBro);
	delete_node(tree);
	return;
}

CellTree* highest_child(CellTree* cell){
	if ((cell->firstChild==NULL) || (cell==NULL)) return NULL;
	CellTree* child= cell->firstChild->nextBro;
	CellTree* max=cell->firstChild;
	while (child!=NULL){
		if (child->height>max->height){
			max=child;
		}
		child=child->nextBro;
	}
	return max;
}

CellTree* last_node(CellTree* tree){
	if ((tree==NULL) || (tree->firstChild==NULL)) return tree;
	return last_node(highest_child(tree));
}

void fusion_cell_protected(CellProtected ** fst, CellProtected ** snd){
//Complexité en linéaire ( omega (taille de fst));
//liste doublement chainée => complexité O(1)
	if (*fst==NULL){
		*fst=*snd;
		return;
	}
	if (*snd==NULL) return;
	CellProtected * tmp= *fst;
	while (tmp->next!=NULL){
		tmp=tmp->next;
	}
	tmp->next=*snd;
}

CellProtected* fusion_highest_child(CellTree * tree){
	CellTree* curr=tree;
	CellProtected *LCP=NULL;
	while (curr!=NULL){	
		fusion_cell_protected(&LCP, &(curr->block->votes));
		curr=highest_child(curr);
	}
	return LCP;
}



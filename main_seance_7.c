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
#include"seance_7.h"

int main()
{
	srand(time(NULL));
	
	char *fichier_voters="keys.txt";
	char *fichier_candidates="candidates.txt";
	char *fichier_decl="declarations.txt";
	
	//Creations des listes
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);

	//Soumission du vote dans le fichier "Pending_votes.txt"
	submit_vote(decl->data);
	
	//Creation du Block qui sera dans la racine comme l'arbre passé en paramètre est NULL.
	CellTree* tree=NULL;
	createBlock(tree,voters->data, 2);	//valeur hachée commencant par 2 zéros
	add_block(2,"Block1.txt");		//Ecriture du Block dans le fichier /Blockchain/Block1.txt
	
	//Liberation de memoire
	delete_tree(tree);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(decl);
	return 0;
}

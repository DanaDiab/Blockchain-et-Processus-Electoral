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
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);

	submit_vote(decl->data);
	CellTree* tree=NULL;
	createBlock(tree,voters->data, 2);
	add_block(2,"Block1.txt");
	
	
	delete_tree(tree);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(decl);
	return 0;
}

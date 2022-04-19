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

/*	CE MAIN CONTIENT LE TEST D'UNE SEULE FONCTION : FUSION_HIGHEST_CHILD	*/


int main()
{
	srand(time(NULL));
	
	char *fichier_voters="keys.txt";
	char *fichier_candidates="candidates.txt";
	char *fichier_decl="declarations.txt";
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);
	CellProtected* decl2=read_protected(fichier_decl);
	CellProtected* decl3=read_protected(fichier_decl);
	CellProtected* decl4=read_protected(fichier_decl);

	//____________________CREATION DES BLOCKS______________
	char hash[20]="hash";
	char pre_hash[20]="preHash";

	Block* b0 = create_block(voters->data,decl,(unsigned char *) hash, (unsigned char * ) pre_hash,1234);
	compute_proof_of_work(b0, 2);
	
	Block* b1 = create_block(voters->next->data, decl2, (unsigned char *)"", b0->hash, 0);
	compute_proof_of_work(b1, 2);

	Block* b2 = create_block(voters->next->next->data, decl3, (unsigned char *)"", b0->hash, 0);
	compute_proof_of_work(b2, 2);

	Block* b3 = create_block(voters->next->next->next->data, decl4, (unsigned char *)"", b2->hash, 0);
	compute_proof_of_work(b3, 2);
	
	//____________________CREATION DES NOEUDS______________
	CellTree *t0 = create_node(b0);
	CellTree *t1 = create_node(b1);
	CellTree *t2 = create_node(b2);
	CellTree *t3 = create_node(b3);
		
	//____________________CREATION DE L'ARBRE______________
	add_child(&t0, t2);
	add_child(&t2, t3);
	add_child(&t0, t1);
	

	//___________________TEST DE FUSION HIGHEST CHILD________
	CellProtected *LCP=fusion_highest_child(t0);
	print_list_protected(LCP);



	delete_list_protected(decl);
	delete_list_protected(decl2);
	b0->votes=NULL;
	b1->votes=NULL;
	b2->votes=NULL;
	b3->votes=NULL;
	delete_tree(t0);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	return 0;
}
	

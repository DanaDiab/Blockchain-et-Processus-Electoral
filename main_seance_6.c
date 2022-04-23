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

int main()
{
	srand(time(NULL));
	
	char *fichier_voters="keys.txt";
	char *fichier_candidates="candidates.txt";
	char *fichier_decl="declarations.txt";
	
	//Declarations des listes
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);
	CellProtected* decl2=read_protected(fichier_decl);
	CellProtected* decl3=read_protected(fichier_decl);

	char hash[20]="hash";
	char pre_hash[20]="preHashhhhhhh";
	
	//creation d'un bloc valide b0
	Block* b0 = create_block(voters->data,decl3,(unsigned char *) hash, (unsigned char * ) pre_hash,1234);
	compute_proof_of_work(b0, 2);
	//creation d'un second block b1 successeur de b0
	Block* b1 = create_block(voters->next->data, decl3, (unsigned char *)"", b0->hash, 0);
	compute_proof_of_work(b1, 2);
	//creattion d'un block b2 qui sera frere de b1
	Block* b2 = create_block(voters->next->next->data, decl3, (unsigned char *)"", b0->hash, 0);
	compute_proof_of_work(b2, 2);
	//creattion d'un block b3 qui sera fils de b2
	Block* b3 = create_block(voters->next->next->next->data, decl3, (unsigned char *)"", b2->hash, 0);
	compute_proof_of_work(b3, 2);
	
	/*
	ARBRE DE TEST :
	
	 [t0]
	 |    \
	 |     \
	[t2]  [t1]
	 |
	 |
	[t3] 
	
	*/
	
	//creation de l'arbre associé
	CellTree *t0 = create_node(b0);
	CellTree *t1 = create_node(b1);
	CellTree *t2 = create_node(b2);
	CellTree *t3 = create_node(b3);
		
	add_child(&t0, t1);
	add_child(&t2, t3);
	add_child(&t0, t2);
	
	//test de l'affichage récursif	
	print_tree(t0);
	
	//test de last_node et highest_child
	printf("\n\nHash du last node de la chaine la plus longue : %s \n", last_node(t0)->block->hash);	

	//test de fusion de listes
	printf("\n____________________________________________________\n");
	printf("FUSION DE LISTES:\n");
	fusion_cell_protected(&decl,&decl2);
	print_list_protected(decl);



	//libération de la mémoire
	//PROVISOIRE LE TEMPS DE COMPRENDRE LA CONSIGNE DE LIBERATIOND DE LA QUESTION 7.9 : on libère en affectant a null le champs votes de chaque bloc après avoir supprimé la liste qui est référencée dans le champs vote de tous les blocs (évite les doubles free). 
	delete_list_protected(decl);
	delete_list_protected(decl3);
	b0->votes=NULL;
	b1->votes=NULL;
	b2->votes=NULL;
	b3->votes=NULL;
	delete_tree(t0);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	return 0;
}

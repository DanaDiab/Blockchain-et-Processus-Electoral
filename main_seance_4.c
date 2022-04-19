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

int main()
{
	srand(time(NULL));
	
	char *fichier_voters="keys.txt";
	char *fichier_candidates="candidates.txt";
	char *fichier_decl="declarations.txt";

	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);

	printf("LISTE VOTANTS :\n");
	print_list_keys(voters);
	printf("***************************\n");

	printf("LISTE CANDIDATS :\n");	
	print_list_keys(candidates);
	printf("***************************\n");

	printf("LISTE DECLARATION AVANT CONTROLE:\n");	
	print_list_protected(decl);		
	printf("***************************\n");
		
	delete_non_valide(&decl);

	printf("LISTE DECLARATION APRES CONTROLE :\n");	
	print_list_protected(decl);
	printf("**************************\n");				
	
	Key* winner=compute_winner(decl, candidates, voters,10, 30);
	char * gagnant=key_to_str(winner);
	printf("Le gagnant des éléctions est : %s \n", gagnant);
	
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(decl);
	free(gagnant);
	free(winner);
	return 0;
}

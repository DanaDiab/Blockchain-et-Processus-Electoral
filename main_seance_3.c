#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"
#include "seance_2.h"
#include "seance_3.h"

int main(){
	srand(time(NULL));
	
	char *fichier="keys.txt";			//nom du fichier contenant les clés publiques et secrètes des votants
	CellKey* LCK=read_public_keys(fichier);		//Creation de la liste chainée de CellKey qui contient les clés publiques dans 'fichier'
	
	print_list_keys(LCK);				//Affichage des datas (Keys) de la liste LCP
	
	delete_list_keys(LCK);				//Libération de la mémoire allouée pour la list LCP


	
	char *fichier1="declarations.txt";		//nom du fichier contenant les votes
	CellProtected* LCP=read_protected(fichier1);	//Creation de la liste chainée de CellProtected qui contient les votes dans 'declarations.txt'
	
	printf("\n\nListe avant controle : \n\n");
	print_list_protected(LCP);			//Affichage de la liste
	
	delete_non_valide(&LCP);			//Suppression des éléments de la liste LCP dont la signature n'est pas valide
	
	printf("\n\nListe après controle : \n\n");
	print_list_protected(LCP);			//Affichage de la liste pour bien s'assurer de la suppression (ce qui a bien été remarqué)
	
	delete_list_protected(LCP);			//Suppression du reste de la liste
	return 0;
}

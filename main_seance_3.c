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
	
	char *fichier="keys.txt";	//nom du fichier dans lequel sont écrit les clés publiques et secrètes des candidats
	CellKey* LCK=read_public_keys(fichier);		//Creation de la liste chainée de CellKey qui contient les valeurs dans 'fichier'
	print_list_keys(LCK);				//Affichage des datas (Keys) de la liste LCP
	delete_list_keys(LCK);				//Libération de la mémoire allouée pour la list LCP

	CellProtected* LCP=read_protected();		//Creation de la liste chainée de CellKey qui contient les valeurs dans 'declarations.txt'
	print_list_protected(LCP);			//Affichage de la liste
	delete_non_valide(&LCP);			//Suppression des éléments de la liste LCP dont la signature n'est pas valide
	print_list_protected(LCP);			//Affichage de la liste pour bien s'assurer de la suppression (ce qui a été approuvé)
	delete_list_protected(LCP);			//Suppression du reste de la liste
	return 0;
}

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_1.h"
#include "seance_2.h"
#include "seance_3.h"

int main(){
	char *fichier="keys.txt";
	CellKey* LCK=read_public_keys(fichier);
	print_list_keys(LCK);
	delete_list_keys(LCK);

	CellProtected* LCP=read_protected();
	print_list_protected(LCP);
	delete_list_protected(LCP);
	return 0;
}
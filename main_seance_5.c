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

int main()
{
	srand(time(NULL));
	
	char *fichier_voters="keys.txt";
	char *fichier_candidates="candidates.txt";
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected();
	
	char hash[10]="hash";
	char pre_hash[10]="preHash";
	Block* b = create_block(voters->data,decl,(unsigned char *) hash, (unsigned char * ) pre_hash,1234);
	
	/* TEST block_to_file */
	block_to_file(b, "test_block.txt");

	delete_list_protected(decl);
	free(b->hash);
	free(b->previous_hash);
	free(b);

	/* TEST file_to_block */
	b=file_to_block("test_block.txt");
	block_to_file(b, "test_block_2.txt");

	/* TEST block_to_str */
	char* b_str=block_to_str(b);
	printf("BLOCK TO STR :\n%s\n", b_str);
	free(b_str);

	delete_list_keys(voters);
	delete_list_keys(candidates);

	delete_list_protected(b->votes);
	free(b->author);
	free(b->hash);
	free(b->previous_hash);
	free(b);

	/*TEST de la fonction de hashage*/
	const char * s= "Rosetta code";
	unsigned char * d = hashage(s);
	printf("Hashage de 'Rosetta code' : \n");
	affichage_hashage(d);
	

	return 0;
}

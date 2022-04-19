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
	char *fichier_decl="declarations.txt";
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);
	
	char hash[20]="hash";
	char pre_hash[20]="preHashhhhhhh";
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



	FILE *fichier=fopen("sortie_compute_proof.txt","w");
	if (fichier==NULL){
		printf("Erreur lors de l'ouverture du fichier \n");
		return -1;
	} 
	for (int i=1;i<5;i++){
		double temps_cpu1=0,temps_init1=0, temps_fin1=0;
		temps_init1= clock();
		compute_proof_of_work(b,i);
		printf("BLOCK VALIDE : %d\n", verify_block(b, i));
		printf("%s\n", b->hash);
		temps_fin1= clock();
		temps_cpu1= ((double)(temps_fin1-temps_init1))/CLOCKS_PER_SEC;
		fprintf(fichier,"%d %lf \n",i,temps_cpu1);
		}
	fclose(fichier);

	/* TEST block_to_str */
	char* b_str=block_to_str(b);
	printf("BLOCK TO STR :\n%s\n", b_str);
	free(b_str);

	delete_list_keys(voters);
	delete_list_keys(candidates);

	delete_list_protected(b->votes);
	free(b->hash);
	free(b->author);
	free(b->previous_hash);
	free(b);

	/*TEST de la fonction de hashage*/
	const char * s= "Rosetta code";
	unsigned char * d = hashage(s);
	printf("Hashage de 'Rosetta code' : \n");
	printf("%s\n", d);
	free(d);

	return 0;
}

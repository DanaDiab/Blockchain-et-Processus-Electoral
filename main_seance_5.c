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
	
	//Declarations
	CellKey* voters=read_public_keys(fichier_voters);
	CellKey* candidates=read_public_keys(fichier_candidates);	
	CellProtected* decl=read_protected(fichier_decl);
	
	//Creation de Block
	char hash[20]="hash";
	char pre_hash[20]="preHashhh";
	Block* b = create_block(voters->data,decl,(unsigned char *) hash, (unsigned char * ) pre_hash,1234);
	
	/* Ecriture d'un Block dans un fichier */
	block_to_file(b, "test_block.txt");

	//Liberation de memoire
	delete_list_protected(decl);
	free(b->hash);
	free(b->previous_hash);
	free(b);

	/* Lecture d'un Block depuis un fichier */
	b=file_to_block("test_block.txt");
	
	//Re-ecriture du même Block dans un nouveau fichier
	block_to_file(b, "test_block_2.txt");


	//TEMPS D'EXECUTION DE LA FONCTION COMPUTE_PROOF_OF_WORK
	FILE *fichier=fopen("sortie_compute_proof.txt","w");		//Ouverture du fichier en mode ecriture
	if (fichier==NULL){						//Erreur d'ouverture
		printf("Erreur lors de l'ouverture du fichier \n");
		return -1;
	} 
	
	double temps_cpu1;
	double temps_init1;
	double temps_fin1;
	for (int i=1;i<5;i++){						//i representant le nb de zéros
		//Declarations
		temps_cpu1=0;
		temps_init1=0; 
		temps_fin1=0;
		temps_init1= clock();					//Temps auquel commence l'execution de compute_proof_of_work
		compute_proof_of_work(b,i);
		temps_fin1= clock();					//Temps auquel l'éxecution se termine
		
		//Verification de la validité du Block et obsertvation de la valeur hachée du Block, commencant par i zéros
		printf("BLOCK VALIDE : %d\n", verify_block(b, i));
		printf("%s\n", b->hash);
		
		//Calcul de la différence du temps en secondes
		temps_cpu1= ((double)(temps_fin1-temps_init1))/CLOCKS_PER_SEC;
		
		//Ecriture dans le fichier
		fprintf(fichier,"%d %lf \n",i,temps_cpu1);
	}
	
	fclose(fichier);						//Fermeture du fichier

	/* Test de Serialisation du Block*/
	char* b_str=block_to_str(b);
	printf("BLOCK TO STR :\n%s\n", b_str);
	
	//Liberation de la mémoire
	free(b_str);
	delete_list_keys(voters);
	delete_list_keys(candidates);
	delete_list_protected(b->votes);
	free(b->hash);
	free(b->author);
	free(b->previous_hash);
	free(b);

	/*TEST de notre fonction de hashage*/
	const char * s= "Rosetta code";
	unsigned char * d = hashage(s);
	printf("Hashage de 'Rosetta code' : \n");
	printf("%s\n", d);
	free(d);

	return 0;
}

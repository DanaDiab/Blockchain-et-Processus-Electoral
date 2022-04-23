#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include <openssl/sha.h>
#include <dirent.h>
#include"seance_1.h"
#include"seance_2.h"
#include"seance_3.h"
#include"seance_4.h"
#include"seance_5.h"
#include"seance_6.h"
#include"seance_7.h"

void submit_vote(Protected *p){					//Ecriture du Protected dans un fichier
	FILE * file=fopen("Pending_votes.txt","a");		//Ouverture du fichier en mode append (ecrire à la fin du fichier sans ecraser)
	
	if (file==NULL){					//Erreur d'ouverture
		printf("Erreur lors de l'ouverture du fichier\n");
		return;
	}
	
	char * str= protected_to_str(p);			//Serialisation du Protected
	fprintf(file,"%s",str);					//Ecriture dans le fichier
	free(str);						
	fclose(file);
	return;
}

void createBlock(CellTree* tree,Key* author, int d){				//Ecriture d'un Block dans un fichier à partir du fichier Pending_votes.txt
	
	CellProtected *LCP=read_protected("Pending_votes.txt");			//Lecture de la liste de vote en attente dans un fichier
	char hash[10]="hash"; 							//Utilisée temprorairement
	char prev_hash[10]=".";							//Utilisée en cas de racine
	CellTree* last=last_node(tree);						//Variable qui stock le dernier noeud de la plus branche de l'arbre
	Block*b=NULL;
	if (last==NULL){							//Tree est NULL => Ce Block est racine
		b=create_block(author, LCP,(unsigned char *)hash,(unsigned char *)prev_hash,0);
	}
	else{									//Previous_hash de block est la valeur hachée du dernier noeud
		b=create_block(author, LCP,(unsigned char *)hash,last->block->hash,0);
	}
	
	compute_proof_of_work(b,d);						//Generation du nonce et de la valeur hachée
	
	//Suppression du fichier
	int del=remove("Pending_votes.txt");					
	if (del==0) printf("Le fichier Pending_votes.txt a été supprimé.\n"); 
	else printf("Le fichier Pending_votes.txt n'a PAS été supprimé.\n");
	
	//Ecriture du Block créé dans le fichier
	block_to_file(b,"Pending_block.txt");

	//Liberation de la mémoire
	b->votes=NULL;
	delete_list_protected(LCP);
	delete_block(b);
	
	return;
}

void add_block(int d,char * name){							//Ajout d'un Block dans le repertoire Blockchain
	Block * block=file_to_block("Pending_block.txt");				//Lecture du Block depuis le fichier
	
	
	//S'assurer si le Block est valide
	if (verify_block(block,d)!=1){							//Si il ne l'est pas				
		printf("Block non valide\n");				
		int del=remove("Pending_block.txt");					//suppression du fichier
		if (del==0) printf("Le fichier Pending_block.txt a été supprimé.\n");
		else printf("Le fichier Pending_block.txt n'a PAS été supprimé.\n");
		return;
	}
	char * str=block_to_str(block);							//Serialisation du Block
	char dir[20]="Blockchain/";
	strcat(dir,name);								//Construction du path du fichier dans lequel ecrire
	block_to_file(block,dir);
	
	////Suppression du fichier
	int del=remove("Pending_block.txt");
	if (del==0) printf("Le fichier Pending_block.txt a été supprimé.\n");
	else printf("Le fichier Pending_block.txt n'a PAS été supprimé.\n");
	
	//Liberation de la memoire
	free(str);
	free(block->author);
	delete_list_protected(block->votes);
	block->votes=NULL;
	delete_block(block);
	
	return;
}

CellTree* read_tree(){
	
	//PARTIE I : Compter le nombre de fichiers dans Blockchain
	DIR * rep_count =opendir("./Blockchain/");		//Ouverture du repertoire
	if (rep_count==NULL) return NULL;			//Erreur d'ouverture
	Block * b =NULL;
	int size=0;
	struct dirent * dir;					//Structure representant un repertoire
	while ((dir = readdir(rep_count))){			//Iteration sur les fichiers du rep
		if ((strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name,"..")!=0)){		//dir->d_name indique le nom du fichier courrant
			size++;
		}
	}
	closedir(rep_count);					//Fermeture du rep
	
	
	//PARTIE II : Creation des Block et des noeuds
	DIR * rep =opendir("./Blockchain/");			//Ouverture du rep
	if (rep==NULL) return NULL;				//Erreur d'ouverture
	CellTree** T=malloc(sizeof(CellTree*)*size);		//Allocation de l'arbre à construire de taille = nb de fichier du rep
	
	if (T!=NULL){						//Si l'allocation réussi
		
		for (int i=0;i<size;i++){			//Initialisation avec des valeurs NULL, pour eviter les segfaults
			T[i]=NULL;
		}
		
		int i=0;
		if (rep!=NULL){					
			struct dirent * dir;
			while ((dir = readdir(rep))){
				if ((strcmp(dir->d_name,".")!=0) && (strcmp(dir->d_name,"..")!=0)){
					
					//Construction du path du fichier qui contient le Block
					char chemin[30]="Blockchain/";
					strcat(chemin,dir->d_name);		
					
					//Lecture et Creation du Block
					b =file_to_block(chemin);
					
					//s'assurer de la creation du Block et du noeud qui le contient avant d'incrementer l'index
					if (b!=NULL){
						T[i]=create_node(b);
						if (T[i]!=NULL) i++;
					}
				}
			}
		}
		closedir(rep);					//Fermeture du rep
		
		//PARTIE III : Construction de l'arbre
		for (int i=0;i<size;i++){					//On fixe un index
			for (int j=0;j<size;j++){				//On parcourt le tableau
				if ((T[i]!=NULL) && (T[j]!=NULL)){
					
					/*Si les deux cases contiennent des Bnoeuds,
					on compare la valeur hachée de l'un avec la valeur hachée du Block précedent de l'autre,
					si elles sont égales alors ils sont père et fils*/
					if (strcmp((char*)T[i]->block->hash,(char*)T[j]->block->previous_hash)==0){
						add_child(&T[i],T[j]);
					}
				}
			}
		}
		
		//PARTIE IV : RECHERCHE DE LA RACINE
		for (int i=0;i<size;i++){	//Parcours du tableau
			if ((T[i]!=NULL) && (T[i]->father==NULL)){	//Case qui contient un noeud et dont le père est NULL est la racine
				CellTree* racine=T[i];	
				free(T);				//Libere le tableau mais pas les noeud qu'il contient.
				return racine;
			}
		}
	}
	return NULL;
}
				
			
Key * compute_winner_BT(CellTree* tree, CellKey * candidates, CellKey* voters,int sizeC, int sizeV){
	CellProtected* LCP=fusion_highest_child(tree);					//Fusionne les listes de votes de tous les noeuds de la plus longue chaine de l'arbre
	delete_non_valide(&LCP);							//Suppression des fraudes de vote
	Key * key=compute_winner(LCP,candidates,voters,sizeC,sizeV);			//Récuperation de Key du gagnant
	return key;
}

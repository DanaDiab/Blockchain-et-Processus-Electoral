#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include <assert.h>
#include"seance_1.h"
#include"seance_2.h"

void init_key(Key* key, long val, long n)	//Initialise un structure Key déjà allouée.
{
	key->k=val;
	key->n=n;
}

void init_pair_keys(Key* pk, Key* sk, long low_size, long up_size)
{
	long p = random_prime_number(low_size, up_size, 5000);
	long q = random_prime_number(low_size, up_size, 5000);
	long n;
	long s;
	long u;
	while(p==q)			// générer deux long premiers et distincts
	{
		p = random_prime_number(low_size, up_size, 5000);
		q = random_prime_number(low_size, up_size, 5000);
	}

	generate_key_values(p, q, &n, &s, &u);		//generer les valeurs n,s,u qui vont servir de clé publique et secrète.

	if (u <0) //pour avoir des clés positives
	{
		long t = (p -1) *( q -1);
	u = u + t ; //on aura toujours s*u mod t = 1
	}

	init_key(pk, s, n);		//initialisation de la clé publique
	init_key(sk, u, n);		//initialisation de la clé secrète

}


char * key_to_str(Key*  key){				//Serialization de Key en chaine de caratère
	char *buffer=(char*)malloc(256*sizeof(char));	//Allocation dynamique de la chaine de caractères.
	if (buffer!=NULL)
		sprintf(buffer, "(%lx,%lx)", key->k,key->n);	//Ecriture des valeurs de la clé dans le buffer sous forme de (k,n)
	return buffer;
}

Key* str_to_key(char* str){				//Transforme une chaine de caractère en Key;
	long k, n;
	sscanf(str,"(%lx,%lx)",&k,&n);			//Lire dans str et stocker les valeurs dans k et n;
	Key * key=(Key*)malloc(sizeof(Key));		//Allocation de la clé
	if (key!=NULL){
		init_key(key,k,n);			//Initialisation de la clé
	}
	return key;
}

Signature* init_signature(long *content, int size)	
{
	Signature *sign=(Signature*)malloc(sizeof(Signature));	//Allocation dynamique d'une signature
	if(sign==NULL){
		printf("Erreur lors de l'allocation\n");	
		return NULL;
	}
	sign->size=size;					//Initialisation
	sign->content=content;

	return sign;
}

Signature* sign(char* mess, Key* sKey)
{
	int size = strlen(mess);
	long *content=encrypt(mess, sKey->k, sKey->n);		//Chiffrage de la signature avec la clé secrète
	Signature *sign = init_signature(content, size);	//Allocation et initialisation 
	return sign;
}


char* signature_to_str(Signature* sgn)	//Serialization de la signature en chaine de caractères
{
	char* result = malloc((10*(sgn->size))*sizeof(char));
	result [0]='#';
	int pos = 1;
	char buffer [256];
	for(int i=0; i<sgn->size; i++)
	{
		sprintf(buffer,"%lx", sgn->content[i]);
		for(int j=0; j< strlen(buffer); j++)
		{
			result[pos] = buffer[j];
			pos = pos +1;
		}
		result[pos] ='#';
		pos = pos+1;
	}
	result[pos] ='\0';
	result = realloc(result , (pos+1)*sizeof(char));
	return result;
}

Signature* str_to_signature(char* str)
{
	int len = strlen(str);
	long* content = (long*)malloc(sizeof(long)+500*len);
	int num = 0;
	char buffer [256];
	int pos = 0;
	for(int i=0; i<len; i++)
	{
		if(str[i] !='#')
		{
			buffer[pos] = str[i];
			pos=pos+1;
		}
		else
		{
			if(pos != 0)
			{
				buffer[pos] ='\0';
				sscanf(buffer,"%lx", &(content[num]));
				num = num + 1;
				pos = 0;
			}
		}
	}
	content=realloc(content ,num*sizeof(long));
	return init_signature(content , num);
}

Protected* init_protected(Key* pkey, char * mess, Signature* sgn){	//Allocation et initialisation de Protected
	Protected * p=malloc(sizeof(Protected));
	if (p!=NULL){
		p->pkey=pkey;
		p->mess=strdup(mess); 		//Allocation et copie de mess
		p->sign=sgn;
	}
	return p;
}

int verify(Protected *pr){		//retourne 1 si la vérification réussi
	char *sign_decrypted=decrypt(pr->sign->content, pr->sign->size, pr->pkey->k, pr->pkey->n); //Dechiffrage de signature
	int v=1;
	if (strcmp(sign_decrypted,pr->mess)!=0){ 		//Comparaison entre la valeur dechiffré et le message initiale
		v=0;
	}
	free(sign_decrypted);					//Desallocation de la memoire
	return v;
}

char * protected_to_str(Protected* p){		//Serialisation de protected en chaine de caractères.
	char *key=key_to_str(p->pkey);
	char*sign=signature_to_str(p->sign);
	char *res=malloc(sizeof(char)*(256*3));
	if (res!=NULL){
		sprintf(res,"%s %s %s\n", key,p->mess,sign); //Ecriture des champs de protected sous forme de char * dans res.
	}
	free(key);
	free(sign);
	return res;
}

Protected * str_to_protected(char *str)
{
	char key[256];
	char signature[256];
	char m[256];
	sscanf(str, "%s %s %s \n", key, m, signature);
	Key* clef = str_to_key(key);
        Signature* sign = str_to_signature(signature);
	return init_protected(clef, m, sign);	//Allocation et Initialisation de protected
}

void generate_random_data(int nv, int nc)
	//Création de 3 fichiers: keys.txt contient les clés publiques et secretes des citoyens
				//candidates.txt contient les clés publiques des candidats
				// declarations.txt contient pour chaque citoyen, sa clé publique, son message et sa signature.
{
	Key* pk = NULL;
	Key* sk = NULL;
	Key* tab_nc[nc];		// tableau qui stock les clés publiques des citoyens
	Key* tab_nv[nv];		// tableau qui stock les clés publiques des candidats
	Key* tab_sk[nv];		// tableau qui stock les clés secretes des citoyens
	FILE *f_keys = fopen("keys.txt", "w");		//Ouverture des fichiers
	FILE *f_candidats=fopen("candidates.txt", "w");
	FILE *f_declarations=fopen("declarations.txt", "w");
	if ((f_keys==NULL) || (f_candidats==NULL) || (f_declarations==NULL)){
		printf("Erreur lors de l'ouverture du fichier\n");
		return;
	}
	char * key_str;
	char * key_str1;
	for(int i=0; i<nv; i++)
	{
		pk=(Key*)malloc(sizeof(Key));		//Allocation dynamique des Keys
		sk=(Key*)malloc(sizeof(Key));
		if ((pk==NULL) || (sk==NULL)){
			printf("Erreur lors de l'allocation\n");
			return;
		}
		init_pair_keys(pk, sk, 3, 7);		//Initialisation des Keys
		key_str=key_to_str(pk);			//Transformation des keys en str
		key_str1=key_to_str(sk);
		fprintf(f_keys, "%s %s\n",key_str,key_str1);	// Ecriture dans le fichier
		free(key_str);
		free(key_str1);
		tab_nv[i]=pk;					//Stockage dans les tableaux
		tab_sk[i]=sk;
	}
	fclose(f_keys);
	int i=0;
	int flag;
	int alea;
	while (i<nc){
		flag=0;
		alea=rand()%(nv);
		for (int j=0;j<i;j++){	//S'assurer que le candidat choisi n'est pas deja dans le tableau des candidats.
			if ((tab_nv[alea]->k==tab_nc[j]->k) && (tab_nv[alea]->n==tab_nc[j]->n)){
					flag=1;
					break;
			}
		}
		if (flag==0){
			tab_nc[i]=tab_nv[alea];		//Ajout du nouveau candidat dans le tableau des candidats
			key_str=key_to_str(tab_nc[i]);
			fprintf(f_candidats, "%s\n", key_str);	//Ecriture du nouveau candidats dans le fichier.
			free(key_str);
			i++;
		}
	}
	fclose(f_candidats);
	Signature *s=NULL;
	Protected *declaration = NULL;
	
	for(int i=0; i<nv; i++)
	{
		alea=rand()%(nc);	//Choix aleatoire du candidat auquel le citoyen va voter.
		char msg[256];
		char *key_str2=key_to_str(tab_nc[alea]);
		sprintf(msg,"%s", key_str2);		 //Le message dans protected
		
		pk=tab_nv[i];			//La clé publique dans protected
		sk=tab_sk[i];
		s=sign(msg, sk);		//La signature dans protected
		declaration=init_protected(pk, msg, s);		//Initialisation de protected  
		char *p_str=protected_to_str(declaration);	//Transformation de protected en *char
		fprintf(f_declarations, "%s", p_str);		//Ecriture dans le fichier
		free(key_str2);					//Liberation des espaces memoires allouées et retournées par les fonctions
		free(s->content);
		free(s);
		free(declaration->mess);
		free(declaration);
		free(p_str);
	}
	fclose(f_declarations);

	for(int i=0; i<nv; i++)
	{
		free(tab_nv[i]);		//Liberation des Keys publiques
		free(tab_sk[i]);		//Liberations des Keys secretes
	}
	
	
}

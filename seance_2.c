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
	if(sign==NULL)return NULL;
	sign->size=size;					//Initialisation
	sign->content=content;

	return sign;
}

Signature* sign(char* mess, Key* sKey)
{
	int size = strlen(mess);
	long *content=encrypt(mess, sKey->k, sKey->n);		//Chiffrage de la signture avec la clé secrète
	Signature *sign = init_signature(content, size);	//Allocation et initialisation 
	if(sign!=NULL)return sign;
	return NULL;
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
	char *sign_decrypted=decrypt(pr->sign->content, pr->sign->size, pr->pkey->k, pr->pkey->n); //
	int v=0;
	if (strcmp(sign_decrypted,pr->mess)==0){
		v=1;
	}
	free(sign_decrypted);
	return v;
}

char * protected_to_str(Protected* p){
	char *key=key_to_str(p->pkey);
	char*sign=signature_to_str(p->sign);
	char *res=malloc(sizeof(char)*(256*3));
	sprintf(res,"%s %s %s\n", key,p->mess,sign);
	free(key);
	free(sign);
	return res;
}

Protected * str_to_protected(char *str)
{
	char key[256];
	char signature[256];
	char m[256];
	char m1[256];
	char m2[256];
	sscanf(str, "%s %s %s %s %s \n", key, m,m1,m2, signature);
	Key* clef = str_to_key(key);
	strcat(m," ");
	strcat(m,m1);
	strcat(m," ");
	strcat(m,m2);
        Signature* sign = str_to_signature(signature);
	return init_protected(clef, m, sign);
}

void generate_random_data(int nv, int nc)
{
	Key* pk = NULL;
	Key* sk = NULL;
	Key* tab_nc[nc];
	Key* tab_nv[nv];
	Key* tab_sk[nv];
	FILE *f_keys = fopen("keys.txt", "w");
	FILE *f_candidats=fopen("candidates.txt", "w");
	FILE *f_declarations=fopen("declarations.txt", "w");
	char * key_str;
	char * key_str1;
	for(int i=0; i<nv; i++)
	{
		pk=(Key*)malloc(sizeof(Key));
		sk=(Key*)malloc(sizeof(Key));
		init_pair_keys(pk, sk, 3, 7);
		key_str=key_to_str(pk);
		key_str1=key_to_str(sk);
		fprintf(f_keys, "%s %s\n",key_str,key_str1);
		free(key_str);
		free(key_str1);
		tab_nv[i]=pk;
		tab_sk[i]=sk;
	}
	fclose(f_keys);

	int alea;
	int flag=0;
	for (int i=0;i<nc;i++){
		flag=0;
		alea=rand()%(nv);
		while (!flag){
			alea=rand()%(nv);
			for (int j=0;j<i;j++){
				if (tab_nv[alea]==tab_nc[j]){
					break;
				}
			}
			if (!flag){
				tab_nc[i]=tab_nv[alea];
				key_str=key_to_str(tab_nc[i]);
				fprintf(f_candidats, "%s\n", key_str);
				flag=1;
				free(key_str);
			}
		}
	}
	fclose(f_candidats);
	Signature *s=NULL;
	Protected *declaration = NULL;
	
	for(int i=0; i<nv; i++)
	{
		alea=rand()%(nc);
		char msg[256];
		char *key_str2=key_to_str(tab_nc[alea]);
		sprintf(msg,"vote pour %s", key_str2); 
		
		pk=tab_nv[i];
		s=sign(msg, pk);
		declaration=init_protected(pk, msg, s);
		char *p_str=protected_to_str(declaration);
		fprintf(f_declarations, "%s", p_str);
		free(key_str2);
		free(s->content);
		free(s);
		free(declaration->mess);
		free(declaration);
		free(p_str);
	}
	fclose(f_declarations);

	for(int i=0; i<nv; i++)
	{
		free(tab_nv[i]);
		free(tab_sk[i]);
	}
	
	
}

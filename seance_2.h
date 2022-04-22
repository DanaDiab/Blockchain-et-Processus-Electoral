#ifndef SEANCE_2
#define SEANCE_2

typedef struct key
{
	long k;
	long n;		//modulo
}Key;

typedef struct signature
{
	int size;
	long *content;
}Signature;

typedef struct protected{
	Key* pkey;	// Public Key du voter
	char * mess;	// Public Key du candidat sous forme de chaine de char
	Signature* sign;
}Protected;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pk, Key* sk, long low_size, long up_size);
char * key_to_str(Key*  key);
Key* str_to_key(char* str);
Signature* init_signature(long *content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
Protected* init_protected(Key * pkey, char * mess, Signature* sgn);
int verify(Protected *pr);
Protected * str_to_protected(char *str);
char* protected_to_str(Protected* p);
void generate_random_data(int nv, int nc);

#endif

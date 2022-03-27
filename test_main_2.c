#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include "seance_2.h"
#include "seance_1.h"

int main(void){
  srand(time(NULL));

  //Testing Init Keys
  Key* pKey=malloc(sizeof(Key));
  Key* sKey= malloc(sizeof(Key));
  init_pair_keys(pKey,sKey,3, 7);
  printf("pKey: %lx, %lx \n",pKey->k,pKey->n);
  printf("sKey: %lx, %lx \n",sKey->k,sKey->n);

  //Testing Key Serialization
  char * chaine=key_to_str(pKey);
  printf("key_to_str: %s \n",chaine);
  Key* k=str_to_key(chaine);
  printf("str_to_key: %lx, %lx \n",k->k,k->n);
	free(chaine);

  //Testing signature
  //Candidate keys :
  Key* pKeyC=malloc(sizeof(Key));
  Key* sKeyC=malloc(sizeof(Key));
  init_pair_keys(pKeyC, sKeyC,3,7);

  //Declaration
  char mess[256]= "vote pour ";
	char *key_str1=key_to_str(pKeyC);
	strcat(mess,key_str1);
	free(key_str1);
	char* pkey_str=key_to_str(pKey);
  printf("%s vote pour %s\n",pkey_str, mess);
	free(pkey_str);
  Signature * sgn=sign(mess,sKey);
  printf("signature: ");
  print_long_vector(sgn->content,sgn->size);
  chaine=signature_to_str(sgn);
  printf("signature_to_str: %s \n",chaine);
	free(sgn->content);
	free(sgn);
  sgn=str_to_signature(chaine);
  printf("str_to_signature: ");
  print_long_vector(sgn->content,sgn->size);

  //Testing protected:
  Protected* pr=init_protected(pKey,mess,sgn);

  //Verification
  if(verify(pr)){
    printf("Signature valide\n");}
  else{
    printf("Signature non valide\n");
  }
	free(chaine);

  chaine=protected_to_str(pr);
  printf("protected_to_str: %s\n",chaine );

	free(pr->mess);
	free(pr);
  pr=str_to_protected(chaine);
	char* key_str=key_to_str(pr->pkey);
	char *sign_str=signature_to_str(pr->sign);
  printf("str_to_protected: %s %s %s\n",key_str,pr->mess,sign_str);

	free(key_str);
	free(sign_str);
	free(chaine);
	free(k);
	free(sgn->content);
	free(pKey);
  free(sKey);
  free(pKeyC);
 	free(sKeyC);
	free(sgn);
	free(pr->pkey);
	free(pr->mess);
	free(pr->sign->content);
	free(pr->sign);
	free(pr);
  return 0;
}

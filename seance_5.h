#ifndef SEANCE_5
#define SEANCE_5

typedef struct block{
	Key * author;
	CellProtected* votes;
	unsigned char* hash;
	unsigned char*  previous_hash;
	int nonce;
}Block;

Block * create_block (Key * author, CellProtected* votes,  unsigned char * hash, unsigned char * prev_hash,int nonce);
void block_to_file(Block* b, char* fichier);
Block* file_to_block(char *fichier);
char* block_to_str(Block *block);
unsigned char * hashage(const char *s);
void affichage_hashage(unsigned char * d);
#endif

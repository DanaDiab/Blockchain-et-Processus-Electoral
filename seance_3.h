#ifndef SEANCE_3
#define SEANCE_3

typedef struct cellKey{
	Key* data;
	struct cellKey* next;
} CellKey;

typedef struct cellProtected{
	Protected * data;
	struct cellProtected * next;
} CellProtected;


CellKey* create_cell_key(Key* key);
void add_cell_key(CellKey** cell, Key * key);
CellKey* read_public_keys(char *fichier);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey * LCK);

CellProtected* create_cell_protected(Protected *pr);
void add_cell_protected(CellProtected **LCP, Protected * pr);
CellProtected* read_protected(char *fic);
void print_list_protected(CellProtected* LCP);
void delete_cell_protected(CellProtected *c);
void delete_list_protected(CellProtected * LCP);
void delete_non_valide(CellProtected ** LCP);
#endif

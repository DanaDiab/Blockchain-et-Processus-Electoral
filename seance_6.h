#ifndef SEANCE_6
#define SEANCE_6

typedef struct block_tree_cell{
	Block* block;
	struct block_tree_cell* father;
	struct block_tree_cell* firstChild;
	struct block_tree_cell* nextBro;
	int height;
} CellTree;

CellTree* create_node(Block *b);
int update_height(CellTree* father,CellTree* child);
void add_child(CellTree** father, CellTree* child);
void affiche_noeud(CellTree *t);
void print_tree(CellTree *t);
void delete_node(CellTree *node);
void delete_tree(CellTree *tree);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
void fusion_cell_protected(CellProtected ** fst, CellProtected ** snd);
CellProtected* fusion_highest_child(CellTree * tree);
#endif

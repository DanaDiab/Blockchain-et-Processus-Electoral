#ifndef SEANCE_7
#define SEANCE_7

void submit_vote(Protected *p);
void createBlock(CellTree* tree,Key* author, int d);
void add_block(int d,char * name);
CellTree* read_tree();
Key * compute_winner_BT(CellTree* tree, CellKey * candidates, CellKey* voters,int sizeC, int sizeV);

#endif

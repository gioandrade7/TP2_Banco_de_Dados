
#include "bpt.hpp"

int main(){

    FILE *arq;
    arq = fopen("BPlusTree.bin", "wb+");

    block *b = create_block(33);
    node *root = create_tree(5, b);

    root = insert(root, 15, 21);
    root = insert(root, 25, 31);
    root = insert(root, 35, 41);
    root = insert(root, 45, 10);

    printTree(root);

   gravaTree(root, 0,arq);
   
   NodeDisk x;
   fseek(arq, 0, SEEK_SET);
   fread(&x, sizeof(NodeDisk), 1, arq);
   imprime_node(x);


}



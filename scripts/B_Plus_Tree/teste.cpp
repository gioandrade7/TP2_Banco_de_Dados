
#include "bpt.hpp"

void imprime_arvore(unsigned long pos, FILE *arq) {
    NodeDisk no;
    fseek(arq, pos, SEEK_SET);
    fread(&no, sizeof(NodeDisk), 1, arq);
    imprime_node(no);
    if (no.is_leaf == 0) 
        for (unsigned int i=0; i <= no.num_keys; i++) {
            imprime_arvore(no.ptrs[i], arq);
        }
}

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
    gravaTree(root, -1, arq);
    fclose(arq);

    cout << endl << endl << "LENDO ARVORE DO ARQUIVO" << endl;
    arq = fopen("BPlusTree.bin", "rb+");
    imprime_arvore(0, arq);
}

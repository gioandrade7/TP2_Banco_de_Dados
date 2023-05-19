
#include "bpt.hpp"

int main(){

    block *b = create_block(1024);
    node *root = create_tree(3, b);

    root = insert(root, 5, 2000);
    root = insert(root, 4, 1810);
    root = insert(root, 8, 1900);
    root = insert(root, 6, 2010);
    root = insert(root, 7, 1750);
    root = insert(root, 9, 2005);

    printTree(root);


}




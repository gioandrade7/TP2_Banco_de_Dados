
#include "bpt.h"

int main(){
    Node no = create_tree();

    add_key_node(&no, 5, 0);
    add_key_node(&no, 0, 7);
    add_key_node(&no, 6, 9);
    add_key_node(&no, 4, 9);
    imprime_node(no);
}




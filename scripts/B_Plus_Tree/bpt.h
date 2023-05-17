#ifndef BPT_H
#define BPT_H

#include<iostream>
#include "../Constantes/cte.h"
#include <bits/stdc++.h>


typedef struct node{

    bool is_leaf; //verifica se é folha
    bool is_root; //verifica se é raiz
    unsigned int keys[2*m]; //vetor com chaves
    unsigned int pointers[2*m+1]; //vetor com ponteiros 
    unsigned int num_keys; //numeros de chaves no nó
    unsigned int parent; //nó pai

}Node;


Node create_tree(){

    Node root;
    root.is_leaf = true;
    root.is_root = true;
    root.num_keys = 0;
    root.parent = 0xffffffff;

    FILE *bpt;
    bpt = fopen("bpt.bin", "w+");

    fwrite(&root, sizeof(Node), 1, bpt);

    fclose(bpt);

    return root;
}

bool add_key_node(Node *no, int k, int ptr){
    if(no->num_keys == 2*m){
        return false;
    }

    if(no->num_keys > 0){
        for(int i = 0; i < no->num_keys; i++){
            if(k < no->keys[i]){
                for(int j = no->num_keys; j > i ; j--){
                    no->keys[j] = no->keys[j - 1];
                }
                no->keys[i] = k;
                if(no->is_leaf){no->pointers[no->num_keys] = ptr;} // se for folha, insere ponteiro para o bucket
                no->num_keys++;
                return true;
            }
        }
    }
    no->keys[no->num_keys] = k;
    if(no->is_leaf){no->pointers[no->num_keys] = ptr;} // se for folha, insere ponteiro para o bucket
    no->num_keys++;
    return true;
}

void imprime_node(Node no){

    cout << "is_leaf: " << no.is_leaf << endl;
    cout << "is_root: " << no.is_root << endl;
    cout << "num_keys: " << no.num_keys << endl;
    cout << "parent: " << no.parent << endl;

    if(no.num_keys > 0){
        for(int i = 0; i < no.num_keys; i++){
            cout << "key " << i << " = " << no.keys[i] << endl;
        }
        for(int i = 0; i < no.num_keys; i++){
            cout << "pointer " << i << " = " << no.pointers[i] << endl;
        }
    }
}

























#endif
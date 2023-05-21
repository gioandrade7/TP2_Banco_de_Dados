#ifndef BPT_HPP
#define BPT_HPP

#include<iostream>
#include "../Constantes/cte.hpp"
#include <bits/stdc++.h>

typedef struct block{
    int offset;
}block;

// Esta struct corresponde aos nós da árvore na memória principal
typedef struct node{
    bool is_leaf; //booleano que indica se o nó é folha
    void **ptrs; //vetor para os nodos filhos, caso não seja folha 
    int *keys; //vetor para as chaves
    struct node *parent; //ponteiro para o nodo pai
    int num_keys; //número de chaves alocadas
    
} node;

// Esta struct corresponde aos nós que serão gravados na memória secundária
typedef struct nodeDisk {
    bool is_leaf; //verifica se é folha
    unsigned long ptrs[2*m+1]; //vetor para os nodos filhos, caso não seja folha 
    unsigned int keys[2*m]; //vetor para as chaves
    unsigned long parent; //ponteiro para o nodo pai
    unsigned int num_keys; //número de chaves alocadas
    
} NodeDisk;

/*
    ESTA FUNÇÃO CALCULA O PONTO DE CORTE (SPLIT) DE UM NÓ DA ÁRVORE
*/
int cut(int size){
    if(size % 2 == 0){return size / 2;}
    else{return size/2 + 1;}
}

/*
    ESTA FUNÇÃO CRIA UM NOVO NÓ VAZIO PARA A ÁRVORE B+
*/
node *create_node(){
    
    node *new_node = (node *)malloc(sizeof(node)); // ALOCA UM NOVO NÓ NA MEMÓRIA
    if(new_node == NULL) {
        perror("Erro ao criar novo nó\n");
        exit(EXIT_FAILURE);
    }

    new_node->keys = (int*) malloc((m*2)*sizeof(int)); // ALOCA O VETOR DE CHAVES
    if(new_node == NULL) {
        perror("Erro ao criar vetor de chaves do novo nó\n");
        exit(EXIT_FAILURE);
    }

    new_node->ptrs = (void**) malloc((2*m + 1)*sizeof(void*)); // ALOCA O VETOR DE APONTADORES
    if(new_node->ptrs == NULL) {
        perror("Erro ao criar vetor de ponteiros do nó\n");
        exit(EXIT_FAILURE);
    }

    new_node->is_leaf = false; // DEFINE O NOVO NÓ COMO NÃO SENDO FOLHA
    new_node->parent = NULL; // DEFINE QUE O NOVO NÓ NÃO TEM UM NÓ PAI
    new_node->num_keys = 0; // DEFINE QUE O NOVO NÓ NÃO ARMAZENA CHAVES

    return new_node;
}

/*
    ESTA FUNÇÃO CRIA UM NOVO NÓ FOLHA PARA A ÁRVORE B+ REUTILIZANDO A FUNÇÃO ANTERIOR
*/
node *create_leaf(){
    node *leaf = create_node(); // CRIA UM NOVO NÓ
    leaf->is_leaf = true; // DEFINE QUE O NÓ É FOLHA
    return leaf;
}

/*
    ESTA FUNÇÃO CRIA UMA NOVA ÁRVORE B+ NA MEMÓRIA PRINCIPAL, INSERINDO UMA CHAVE NO BLOCO RAIZ (ROOT)
*/
node *create_tree(int key, block *b){

    node *root = create_leaf(); // CRIA O NÓ RAIZ COMO SENDO FOLHA, POIS NÃO EXISTE OUTROS NÓS INTERNOS
    root->keys[0] = key; // ARMAZENA A CHAVE NA PRIMEIRA POSIÇÃO DO NÓ RAIZ
    root->ptrs[0] = b; // ARMAZENA O RESPECTIVO OFFSET DA CHAVE (APONTADOR PARA O REGISTRO NO HASH)
    root->parent = NULL;
    root->num_keys++; // ATUALIZA O CONTADOR DE CHAVES NO NÓ
    root->ptrs[2*m] = NULL; //seta o utlimo ponteiro para NULL, já que nao tem filho

    return root;
}

/*
    ESTA FUNÇÃO CONSTRÓI UM BLOCO PARA ARMAZENAR O OFFSET DE UMA CHAVE
*/
block *create_block(int offset){
    block *b = (block*) malloc(sizeof(block));
    if(b == NULL){
        perror("Erro ao criar bloco !\n");
        exit(EXIT_FAILURE);
    }
    else{b -> offset = offset;}
    return b;
}

/*
    ESTA FUNÇÃO EFETUA A BUSCA PELA FOLHA ONDE UMA CHAVE DEVE SER INSERIDA
*/
node *find_leaf(node* root, int key){

    int i = 0;
    node *current = root; //começa a busca na raiz

    while (!current->is_leaf) {     // A BUSCA PERMANECE NAVEGANDO PELA ÁRVORE ATÉ QUE O NÓ ATUAL SEJA UMA FOLHA
        i = 0;
        while (i < current->num_keys) { // PERCORRE AS CHAVES DO NÓ ATUAL, ACHANDO PARA QUAL PRÓXIMO NÓ DEVE AVANÇAR
            if(key >= current->keys[i]) {
                i++;
            }
            else{
                break;
            }
        }
        current = (node *)current->ptrs[i]; // ALTERA O NÓ ATUAL PARA O PRÓXIMO, DE ACORDO COM A CHAVE INFORMADA
    }
    return current;
}

/*
    ESTA FUNÇÃO FAZ A INSERÇÃO DE UMA CHAVE NUM NÓ FOLHA ONDE EXISTE ESPAÇO DISPONÍVEL
*/
node *insert_leaf(node *leaf, int key, block* b){
    for(int i = 0; i < leaf->num_keys; i++) { // PERCORRE AS CHAVES DA FOLHA, BUSCA O LOCAL PARA INSERIR A NOVA CHAVE
        if(key < leaf->keys[i]) { // SE ENCONTROU O LOCAL ENTRE AS CHAVES EXISTENTES
            for(int j = leaf->num_keys; j > i ; j--) { // DESLOCA AS CHAVES DO PONTO DE INSERÇÃO EM DIANTE, UMA POSIÇÃO A FRENTE
                leaf->keys[j] = leaf->keys[j - 1];
                leaf->ptrs[j] = leaf->ptrs[j-1];
            }
            leaf->keys[i] = key; // INSERE A NOVA CHAVE NA POSIÇÃO CORRETA
            leaf->ptrs[i] = b; 
            leaf->num_keys++;
            return leaf;
        }
    }
    leaf->keys[leaf->num_keys] = key; // CASO CONTRÁRIO A NOVA CHAVE SERÁ INSERIDA AO FINAL
    leaf->ptrs[leaf->num_keys] = b;
    leaf->num_keys++;
    return leaf;
}

/*
    ESTA FUNÇÃO FAZ A INSERÇÃO NUM NOVO NÓ PAI (ROOT), RECEBENDO OS NÓS FILHOS A ESQUERDA E A DIREITA DA CHAVE 
*/
node *insert_new_root(node *left, int key, node *right){
    node *root = create_node(); // CRIA O NÓ PAI (ROOT)
    root->keys[0] = key; // INSERE A NOVA CHAVE
    root->ptrs[0] = left; // ATUALIZA O APONTADOR A ESQUERDA (MEMOR QUE A CHAVE)
    root->ptrs[1] = right; // ATUALIZA O APONTADOR A DIREITA (MAIOR OU IGUAL A CHAVE)
    root->num_keys++; // ATUALIZA O NÚMERO DE CHAVES DO NÓ PAI (ROOT)
    root->parent = NULL;
    left->parent = root; // VINCULA O NÓ PAI COM O FILHO A ESQUERDA
    right->parent = root; // VINCULA O NÓ PAI COM O FILHO A DIREITA
    return root;
}

int get_left_index(node *parent, node *left){
    int r = 0;
    while(r <= parent->num_keys && (node*) parent->ptrs[r] != left){r++;}
    return r;
}

/*
    ESTA FUNÇÃO INSERE UMA NOVA CHAVE NUMA POSIÇÃO ESPECÍFICA DE UM NÓ INTERNO (INDEX) E FAZ ATULIZA O APONTADOR PARA O NÓ FOLHA
*/
node *insert_node(node *root, node *n, int left_index, int key, node *right){
    for(int i = n->num_keys; i > left_index; i--){
        n->ptrs[i + 1] = n->ptrs[i];
        n->keys[i] = n->keys[i-1];
    }

    n->ptrs[left_index + 1] = right;
    n->keys[left_index] = key;
    n->num_keys++;
    return root;
}

node *insert_parent(node* root, node *left, int key, node *right);

/*
    ESTA FUNÇÃO FAZ O SPLIT DE UM NÓ INTERNO E INSERE UMA NOVA CHAVE
*/
node *insert_node_after_split(node *root, node *old_node, int left_index, int key, node *right){

    int *temp_keys; //vetor temporário para guardar as chaves
    void **temp_ptrs; // vetor temporário para guardar os ponteiros dos blocks

    temp_keys = (int *) malloc((2*m+1)*sizeof(int));
    if(temp_keys == NULL){
        perror("Erro ao criar vetor de chaves temporário !\n");
        exit(EXIT_FAILURE);
    }

    temp_ptrs = (void**)malloc((2*m+2)*sizeof(void *));
    if(temp_ptrs == NULL){
        perror("Erro ao criar vetor de ponteiros para blocks temporário !\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) { // EFETUA A CÓPIA DOS APONTADORES DO NÓ ANTIGO PARA O VETOR TEMPORÁRIO
        if (j == left_index + 1) { // SALTA A POSIÇÃO ONDE O APONTADOR DA NOVA CHAVE DEVA SER INSERIDO
            j++;
        }
        temp_ptrs[j] = old_node->ptrs[i];
    }

    for (int i = 0, j = 0; i < old_node->num_keys; i++, j++) { // EFETUA A CÓPIA DAS CHAVES DO NÓ ANTIGO PARA O VETOR TEMPORÁRIO
        if (j == left_index) { // SALTA A POSIÇÃO ONDE A NOVA CHAVE DEVA SER INSERIDA
            j++;
        }
        temp_keys[j] = old_node->keys[i];
    }
    // INSERE A NOVA CHAVE E SEU APONTADOR NA POSIÇÃO CORRETA DOS VETORES
    temp_ptrs[left_index + 1] = right;
    temp_keys[left_index] = key;

    int split = cut(2*m+1); // CALCULA O PONTO DE SPLIT
    node * new_node = create_node(); // CRIA O NOVO NÓ PARA RECEBER METADE DAS CHAVES DO NÓ ANTIGO
    old_node->num_keys = 0;
    int i = 0;
    for (i = 0; i < split - 1; i++) { // COPIA AS CHAVES DE VOLTA PARA O NÓ, SOMENTE AS QUE ESTÃO ANTES DO PONTO DE SPLIT
        old_node->ptrs[i] = temp_ptrs[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }
    old_node->ptrs[i] = temp_ptrs[i];
    int k_prime = temp_keys[split - 1];
    int j = 0;
    for (++i, j = 0; i < 2*m+1; i++, j++) { // COPIA AS CHAVES PARA O NOVO NÓ, AQUELAS QUE ESTÃO APÓS O PONTO DE SPLIT
        new_node->ptrs[j] = temp_ptrs[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }
    new_node->ptrs[j] = temp_ptrs[i];

    free(temp_keys);
    free(temp_ptrs);
    
    new_node->parent = old_node->parent;
    for (i = 0; i <= new_node->num_keys; i++) {
        node *child = (node *)new_node->ptrs[i];
        child->parent = new_node;
    }

    return insert_parent(root, old_node, k_prime, new_node);
}

/*
    ESTÁ FUNÇÃO FAZ A INSERÇÃO DE UMA CHAVE NUM NÓ PAI
*/
node *insert_parent(node* root, node *left, int key, node *right){

    node *parent = left->parent;

    if (parent == NULL) { 
        return insert_new_root(left, key, right);
    }

    int left_index = get_left_index(parent, left); //indice do ponteiro do nó à esquerda

    if(parent->num_keys < 2*m) {
        return insert_node(root, parent, left_index, key, right);
    }

    return insert_node_after_split(root, parent, left_index, key, right);
}

/*
    ESTA FUNÇÃO FAZ O SPLIT DE UMA NÓ FOLHA PARA UM NÓ FOLHA SUCESSOR E INSERE UMA NOVA CHAVE
*/
node *insert_leaf_after_split(node *root, node *leaf, int key, block *b){

    node *new_leaf = create_leaf(); //cria nova folha para o split
    int *temp_keys; //vetor temporário para guardar as chaves
    void **temp_ptrs; // vetor temporário para guardar os ponteiros dos blocks

    temp_keys = (int *) malloc((2*m+1)*sizeof(int)); // ALOCA UM VETOR TEMPORÁRIO PARA AS CHAVES
    if (temp_keys == NULL) {
        perror("Erro ao criar vetor de chaves temporário !\n");
        exit(EXIT_FAILURE);
    }

    temp_ptrs = (void **) malloc((2*m+1)*sizeof(void *)); // ALOCA UM VETOR TEMPORÁRIO PARA OS APONTADORES
    if (temp_ptrs == NULL) {
        perror("Erro ao criar vetor de ponteiros para blocks temporário !\n");
        exit(EXIT_FAILURE);
    }

    int insert = 0;
    while (insert < 2*m && key > leaf->keys[insert]) { // ENCONTRA A POSIÇÃO PARA INSERIR A NOVA CHAVE 
        insert ++; // enquanto não chegar ao fim e a chave na posição insert for >= a chave nova, avança
    }

    for(int i, j = 0 ; i < leaf->num_keys; i++, j++) { // COPIA AS CHAVES E APONTADORES, PULANDO A POSIÇÃO ONDE A NOVA CHAVE DEVA SER INSERIDA
        if (j == insert) { j++; }
        temp_keys[j] = leaf->keys[i];
        temp_ptrs[j] = leaf->ptrs[i]; 
    }
    temp_keys[insert] = key; // INSERE A CHAVE NO VETOR TEMPORÁRIO
    temp_ptrs[insert] = b; // INSERE O BLOCO (OFFSET PARA O HASH) NO VETOR DE APONTADORES

    leaf->num_keys = 0; // RESETA O CONTADOR DE CHAVES DO NÓ FOLHA

    int split = cut(2*m); // CALCULA O PONTO DE SPLIT

    for(int i = 0; i < split; i++) { // EFETUA A COPIA DAS CHAVES E APONTADORES ATÉ O PONTO DE SPLIT PARA O NÓ FOLHA PREDECESSOR
        leaf->keys[i] = temp_keys[i];
        leaf->ptrs[i] = temp_ptrs[i];
        leaf->num_keys++;
    }

    for(int i = split, j = 0; i < 2*m + 1; i++, j++) { // EFETUA A CÓPIA DAS CHAVES E APONTADORES APÓS O PONTO DE SPLIT PARA O NÓ FOLHA SUCESSOR
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->ptrs[j] = temp_ptrs[i];
        new_leaf->num_keys++;
    }

    free(temp_keys);
    free(temp_ptrs);

    new_leaf->ptrs[2*m] = leaf->ptrs[2*m]; // ENCADEIA OS NÓS FOLHAS
    leaf->ptrs[2*m] = new_leaf;

    for(int i = leaf->num_keys; i <  2*m; i++) { leaf->ptrs[i] = NULL; } // ATUALIZA OS APONTADORES DA FOLHA PREDECESSORA
    for (int i = new_leaf->num_keys; i < 2*m; i++) { new_leaf->ptrs[i] = NULL; } // ATUALIZA OS APONTADORES DA FOLHA SUCESSORA

    new_leaf->parent = leaf->parent; // VINCULA A FOLHA SUCESSORA COM O MESMO NÓ PAI DA FOLHA PREDECESSORA
    int new_key = new_leaf->keys[0];

    return insert_parent(root, leaf, new_key, new_leaf);
}

/*
    FUNÇÃO PRINCIPAL DE INSERÇÃO DE UMA CHAVE E SEU RESPECTIVO OFFSET (APONTADOR PARA O HASH).
*/
node *insert(node *root, int key, int offset){

    block *b = create_block(offset); //ponteiro onde o b esta alocado
    node *leaf = find_leaf(root, key); // acha a folha onde a chave com o offset do bloco vao ser inseridos

    if(leaf -> num_keys < 2*m){ //ainda tem espaço na folha
        leaf = insert_leaf(leaf, key, b);
        return root;
    }
    return insert_leaf_after_split(root, leaf, key, b);
}

/*
    FUNÇÃO UTILITÁRIA PARA A FUNÇÃO DE IMPRESSÃO DA ÁRVORE NA MEMÓRIA
*/
int pathToLeaves(node *const root, node *child) {
  int length = 0;
  node *c = child;
  while (c != root) {
    c = c->parent;
    length++;
  }
  return length;
}


/*
    IMPRIME AS INFORMAÇÕES DE UM NÓ DA ÁVORE B+ QUE FOI SALVO NO ARQUIVO (NodeDisk).
*/
void imprime_node(NodeDisk no){
    cout << "\tparent pos: " << no.parent << endl;
    cout << "\tis_leaf: " << no.is_leaf << endl;
    cout << "\tnum_keys: " << no.num_keys << endl;
    cout << "\tkeys: \n\t\t";
    for (unsigned int i=0; i < 2*m; i++)
        cout << "(" << i << ":" << no.keys[i] << "), ";
    cout << endl;
    cout << "\tptrs: \n\t\t";
    for (unsigned int i=0; i <= 2*m; i++)
        cout << "(" << i << ":" << no.ptrs[i] << "), ";
    cout << endl << endl;
}

/*
    BUSCA UMA CHAVE DENTRO DE UMA ÁRVORE B+ ARMAZENADA NUM ARQUIVO 
*/
unsigned long search_key(unsigned int key, unsigned long pos, unsigned int *n_faccess, FILE *arq) {
    NodeDisk no; // NÓ ATUAL LIDO DO ARQUIVO
    unsigned int i;
    
    fseek(arq, pos, SEEK_SET); // POSICIONA O CURSOR NO POSIÇÃO DO NÓ A SER LIDO
    fread(&no, sizeof(NodeDisk), 1, arq); // EFETUA A LEITURA DO NÓ
    *n_faccess = *n_faccess + 1; // ATUALIZA O CONTADOR DE ACESSOS AOS DISCO
    imprime_node(no); // IMPRIME O NÓ LIDO

    // SE O NÓ ATUAL NÃO É FOLHA, DEVEMOS COMPARAR A KEY COM O VETOR DE CHAVES DO NÓ E ENCONTRAR QUAL O PRÓXIMO NÓ A SER ANALISADO
    if (no.is_leaf == 0) {
        // PERCORRE O VETOR DE CHAVES DO NÓ, SE A KEY FOI MENOR QUE A CHAVE ATUAL, ENTÃO PEGAMOS O APONTADOR DO NÓ CORRESPONDENTE
        for (i=0; i < no.num_keys; i++)
            if (key < no.keys[i]) 
                return search_key(key, no.ptrs[i], n_faccess, arq);
        // SE FORAM VERIFICADAS TODAS AS CHAVES, E A KEY É MAIOR OU IGUAL A ÚLTIMA CHAVE DO VETOR, ENTÃO O ÚLTIMO APONTADOR SERÁ UTILIZADO NA BUSCA (ÚLTIMO NÓ FILHO)
        if (key >= no.keys[i-1])
            return search_key(key, no.ptrs[i], n_faccess, arq);
    }
    
    // SE O NÓ ATUAL É UMA FOLHA, TEMOS QUE COMPARAR A KEY COM AS CHAVES NO VETOR DO NÓ
    for (i=0; i < no.num_keys; i++) {
        if (key == no.keys[i])
            return no.ptrs[i]; // SE A KEY FOI ENCONTRADA, ENTÃO RETORNAMOS O RESPECTIVO APONTADOR (OFFSET DO HASH)
    }
    
    return -1; // CASO CONTRÁRIO, RETORNAMOS -1 PARA INDICAR QUE A KEY NÃO FOI ENCONTRADA
}

/*
    RECEBE O ARQUIVO COM A ÁRVORE B+ (ÍNDICE SECUNDÁRIO) E IMPRIME TODOS OS NÓS POR LARGURA
*/
void imprime_arvore(unsigned long pos, FILE *arq) {
    NodeDisk no; // nó atual
    fseek(arq, pos, SEEK_SET); // posiciona o cursor na posição do nó a ser lido
    fread(&no, sizeof(NodeDisk), 1, arq); // efetua a leitura do nó
    imprime_node(no); // imprime o nó lido
    if (no.is_leaf == 0) // se o nó não for uma folha, chama a função recursivamente para cada nó interno
        for (unsigned int i=0; i <= no.num_keys; i++) {
            imprime_arvore(no.ptrs[i], arq);
        }
}

/*
    FUNÇÃO UTILITÁRIA QUE IMPRIME A ÁRVORE B+ NA MEMÓRIA.
*/
void printTree(node *const root){

    queue<node*> q;
    node *c = NULL;
    int level = 0;
    int new_level = 0;

    q.push(root);

    while(!q.empty()){
        c = q.front();
        q.pop();

        if (c->parent != NULL && c == c->parent->ptrs[0]) {
            new_level = pathToLeaves(root, c);
            if (new_level != level) {
                level = new_level;
                printf("\n");
            }
        }
        for(int i = 0; i < c->num_keys; i++){
            cout << c->keys[i] << " ";
        }
        if(!c->is_leaf){
            for(int i = 0; i <= c->num_keys; i++){
                q.push((node *)c->ptrs[i]);
            }
        }
        cout << "| ";
    }
    cout << endl;
}

/*
 GRAVA A ÁRVORE B+ NUM ARQUIVO NO DISCO
*/
unsigned long gravaTree(node *root, unsigned long parent, FILE *arq){
    
    NodeDisk temp; // NÓ TEMPORÁRIO QUE IRÁ RECEBER OS DADOS DO NÓ EM MEMÓRIA A SER GRAVADO NO DISCO

    unsigned long pos = ftell(arq); // RECUPERA A POSIÇÃO ATUAL DO ARQUIVO
    temp.is_leaf = root->is_leaf; // COPIA OS ATRIBUTOS DO NÓ EM MEMÓRIA PARA O NÓ A SER GRAVADO EM DISCO
    temp.num_keys = root->num_keys;
    temp.parent = parent;

    // FAZ A CÓPIA DAS CHAVES EXISTENTES, E AS POSIÇÕES NÃO OCUPADAS SÃO PREENCHIDAS COM -1
    for (unsigned int i=0; i < (2*m); i++) {
        if (i < temp.num_keys)
            temp.keys[i] = root->keys[i];
        else
            temp.keys[i] = -1;
    }

    // FAZ A CÓPIA DOS APONTADORES, SE O NÓ FOR FOLHA ELES NÃO SERÃO ALTERADOS POIS CORRESPONDEM A OFFSETS DO HASH, CASO CONTRÁRIO OS APONTADORES SERÃO ATUALIZAODS COM OFFSETS DOS NÓS FILHOS NO ARQUIVO
    for (unsigned int i=0; i <= (2*m); i++) {
        if (i <= temp.num_keys) {
            temp.ptrs[i] = (unsigned long) root->ptrs[i];
        } else {
            temp.ptrs[i] = -1;
        }
    }
    
    fwrite(&temp, sizeof(NodeDisk), 1, arq); // GRAVA O NÓ ATUAL NO ARQUIVO

    // SE O NÓ ATUAL FOR UMA FOLHA, NÃO EXISTE MAIS NADA A SER FEITO E A FUNÇÃO RETORNA A POSIÇÃO ONDE O NÓ FOI GRAVADO
    if (temp.is_leaf == 1) {
        return pos; 
    } 
    
    // SE O NÓ ATUAL NÃO FOR UMA FOLHA, É PRECISO SALVAR SEUS NÓS FILHOS E ATUALIZAR OS VALORES DOS APONTADORES PARA OS RESPECTIVOS OFFSETS DOS NÓS FILHOS
    for (unsigned int i=0; i <= (2*m); i++) {
        if (i <= temp.num_keys) {
            temp.ptrs[i] = gravaTree((node *)root->ptrs[i], pos, arq); // CHAMA A FUNÇÃO RECURSIVAMENTE PARA GRAVAR OS NÓS FILHOS NO ARQUIVO
        } else {
            temp.ptrs[i] = -1; // SE NÃO EXISTE CHAVE ASSOCIADO AO APONTAR ENTÃO ELE RECEBE O VALOR -1 (NULL)
        }
    }

    // APÓS ATUALIZAR OS APONTADORES COM OS OFFSETS DOS NÓS FILHOS NO ARQUIVO, É PRECISO REESCREVER O NÓ ATUAL NO ARQUIVO
    fseek(arq, pos, SEEK_SET); // REPOSICIONA O CURSOR DO ARQUIVO NO INÍCIO DO NÓ ATUAL
    fwrite(&temp, sizeof(NodeDisk), 1, arq); // SOBREESCREVE O NÓ ATUAL COM OS APONTADORES ATUALIZADOS
    fseek(arq, 0, SEEK_END); // MOVE O CURSOR DO ARQUIVO NOVAMENTE PARA O FINAL
    
    return pos; // RETORNA A POSIÇÃO EM QUE FOI SALVO O NÓ ATUAL
}

#endif

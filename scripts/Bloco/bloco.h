#ifndef BLOCO_H
#define BLOCO_H

#include <bits/stdc++.h>
#include "../Constantes/cte.h"
#include "../Registro/registro.h"

using namespace std;

typedef struct HeaderBlock{
    int espacoLivre; //Tamanho do bloco em bytes
    int numeroRegistros; //Quantidade de registros no bloco
    vector<int> offsetRegistros; //Guarda o valor do offset do próximo registro
} HeaderBlock;

typedef struct Bloco{
    //Header do Bloco
    HeaderBlock header;
    //Dados do Bloco
    char bloco[block_size];
} Bloco;

Bloco createBloco(){
    Bloco bloco;
    bloco.header.espacoLivre = block_size;
    bloco.header.numeroRegistros = 0;
    return bloco;
}

Bloco ExtrairBloco(char *bloco){
    Bloco blocoExtraido;
    int pos = 0;
    memcpy(&blocoExtraido.header.espacoLivre, bloco, sizeof(int));
    pos += sizeof(int);
    memcpy(&blocoExtraido.header.numeroRegistros, bloco + pos, sizeof(int));
    pos += sizeof(int);
    for(int i = 0; i < blocoExtraido.header.numeroRegistros; i++){
        int offset;
        memcpy(&offset, bloco + pos, sizeof(int));
        pos += sizeof(int);
        blocoExtraido.header.offsetRegistros.push_back(offset);
    }
    memcpy(&blocoExtraido.bloco, bloco, block_size);
    return blocoExtraido;
}

void printBloco(Bloco *bloco){
    cout << "Espaço Livre: " << bloco->header.espacoLivre << endl;
    cout << "Numero de Registros: " << bloco->header.numeroRegistros << endl;
    cout << "Offset dos Registros: ";
    for(int i = 0; i < bloco->header.numeroRegistros; i++){
        cout << bloco->header.offsetRegistros[i] << " ";
    }
    cout << endl;
}

// Bloco insertInBloco(Registro *registro, Bloco *bloco){
//     if(bloco->header.espacoLivre < registro->tamanhoRegistro){
//         return false;
//     }

//     bloco->header.espacoLivre -= registro->tamanhoRegistro;
//     bloco->header.numeroRegistros++;
//     bloco->header.tamanhoHeader += sizeof(int);
//     bloco->header.offsetRegistros.push_back(bloco->header.tamanhoHeader + registro->tamanhoRegistro);
    
// }

#endif
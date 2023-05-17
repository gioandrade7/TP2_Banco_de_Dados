#ifndef BLOCO_HPP
#define BLOCO_HPP

#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <cstring>
#include "../Constantes/cte.hpp"
#include "../Registro/registro.hpp"

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
    unsigned char blocoBytes[BLOCO_SIZE];
} Bloco;

Bloco createBloco(){
    Bloco bloco;
    bloco.header.espacoLivre = BLOCO_SIZE - 2*sizeof(int); //Tirando o espaço ocupado pelo header
    bloco.header.numeroRegistros = 0;

    bloco.blocoBytes[0] = bloco.header.espacoLivre;
    bloco.blocoBytes[1] = bloco.header.numeroRegistros;

    for(int i=2;i<BLOCO_SIZE;i++){
        bloco.blocoBytes[i] = 0;
    } 
    return bloco;
}


Bloco extrairHeader(char *blocoBytes){
    Bloco bloco;
    int pos = 0;
    memcpy(&bloco.header.espacoLivre, blocoBytes, sizeof(int));
    pos += sizeof(int);
    memcpy(&bloco.header.numeroRegistros, blocoBytes + pos, sizeof(int));
    pos += sizeof(int);
    for(int i = 0; i < bloco.header.numeroRegistros; i++){
        int offset;
        memcpy(&offset, blocoBytes + pos, sizeof(int));
        pos += sizeof(int);
        bloco.header.offsetRegistros.push_back(offset);
    }
    // Copia o vetor de bytes para o vetor do bloco
    memcpy(bloco.blocoBytes, blocoBytes, BLOCO_SIZE);
    return bloco;
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

bool inserirRegistro(Bloco *bloco, Registro &registro) {

    if (bloco->header.espacoLivre < registro.tamanhoRegistro + sizeof(int)) {
        return false;
    }

    int posicaoInsercao;

    if(bloco->header.numeroRegistros == 0){
        posicaoInsercao = BLOCO_SIZE - registro.tamanhoRegistro;
    }
    else{
        posicaoInsercao = bloco->header.offsetRegistros.back() - registro.tamanhoRegistro;
    }
    int novoOffset = posicaoInsercao;

    //memcpy(&bloco->blocoBytes[posicaoInsercao], &registro, registro.tamanhoRegistro);
    //posicaoInsercao += registro.tamanhoRegistro;

    // Copia os dados do registro para o vetor do bloco
    memcpy(&bloco->blocoBytes[posicaoInsercao], &registro.id, sizeof(int));
    posicaoInsercao += sizeof(int);

    memcpy(&bloco->blocoBytes[posicaoInsercao], &registro.tamanhoRegistro, sizeof(int));
    posicaoInsercao += sizeof(int);

    memcpy(&bloco->blocoBytes[posicaoInsercao], &registro.ano, sizeof(int));
    posicaoInsercao += sizeof(int);

    memcpy(&bloco->blocoBytes[posicaoInsercao], &registro.citacoes, sizeof(int));
    posicaoInsercao += sizeof(int);

    memcpy(&bloco->blocoBytes[posicaoInsercao], registro.atualizacao.c_str(), registro.atualizacao.size()+1);
    posicaoInsercao += registro.atualizacao.size()+1;

    memcpy(&bloco->blocoBytes[posicaoInsercao], registro.titulo.c_str(), registro.titulo.size()+1);
    posicaoInsercao += registro.titulo.size()+1;

    memcpy(&bloco->blocoBytes[posicaoInsercao], registro.autores.c_str(), registro.autores.size()+1);
    posicaoInsercao += registro.autores.size()+1;

    memcpy(&bloco->blocoBytes[posicaoInsercao], registro.snippet.c_str(), registro.snippet.size()+1);
    posicaoInsercao += registro.snippet.size()+1;

    // Atualiza o header do bloco
    bloco->header.espacoLivre -= registro.tamanhoRegistro;
    bloco->header.numeroRegistros++;

    // Adiciona o novo offset do registro no vetor de offsets
    bloco->header.offsetRegistros.push_back(novoOffset);

    posicaoInsercao = 0;

    //memcpy(&bloco->blocoBytes[posicaoInsercao], &bloco->header, sizeof(int));


    // Copia as informações do header para o vetor de bytes do bloco
    memcpy(&bloco->blocoBytes[posicaoInsercao], &bloco->header.espacoLivre, sizeof(int));
    posicaoInsercao += sizeof(int);

    memcpy(&bloco->blocoBytes[posicaoInsercao], &bloco->header.numeroRegistros, sizeof(int));
    posicaoInsercao += sizeof(int);

    // Copia os offsets dos registros para o vetor de bytes do bloco
    for (int i = 0; i < bloco->header.numeroRegistros; i++) {
        memcpy(&bloco->blocoBytes[posicaoInsercao], &bloco->header.offsetRegistros[i], sizeof(int));
        posicaoInsercao += sizeof(int);
    }

    return true;
}

#endif
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

    for(int i=0;i<BLOCO_SIZE;i++){
        bloco.blocoBytes[i] = 0;
    }
    return bloco;
}


Bloco extrairHeader(char *blocoBytes){
    Bloco bloco;
    int pos = 0;
    memcpy(&bloco.header.espacoLivre, &blocoBytes[pos], sizeof(int));
    pos += sizeof(int);
    memcpy(&bloco.header.numeroRegistros, &blocoBytes[pos], sizeof(int));
    pos += sizeof(int);

    for(int i = 0; i < bloco.header.numeroRegistros; i++){
        int offset;
        memcpy(&offset, &blocoBytes[pos], sizeof(int));
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

bool insertRegistroBloco(Bloco *bloco, Registro &registro) {

    if (bloco->header.espacoLivre < registro.tamanhoRegistro + sizeof(int)) {
        return false;
    }

    int address;

    if(bloco->header.numeroRegistros == 0){
        address = BLOCO_SIZE - registro.tamanhoRegistro;
    }
    else{
        address = bloco->header.offsetRegistros.back() - registro.tamanhoRegistro;
    }
    int novoOffset = address;
    //memcpy(&bloco->blocoBytes[address], &registro, registro.tamanhoRegistro);
    //address += registro.tamanhoRegistro;

    // Copia os dados do registro para o vetor do bloco
    memcpy(&bloco->blocoBytes[address], &registro.id, sizeof(int));
    address += sizeof(int);

    memcpy(&bloco->blocoBytes[address], &registro.tamanhoRegistro, sizeof(int));
    address += sizeof(int);

    memcpy(&bloco->blocoBytes[address], &registro.ano, sizeof(int));
    address += sizeof(int);

    memcpy(&bloco->blocoBytes[address], &registro.citacoes, sizeof(int));
    address += sizeof(int);

    memcpy(&bloco->blocoBytes[address], registro.atualizacao.c_str(), registro.atualizacao.size()+1);
    address += registro.atualizacao.size()+1;

    memcpy(&bloco->blocoBytes[address], registro.titulo.c_str(), registro.titulo.size()+1);
    address += registro.titulo.size()+1;

    memcpy(&bloco->blocoBytes[address], registro.autores.c_str(), registro.autores.size()+1);
    address += registro.autores.size()+1;

    memcpy(&bloco->blocoBytes[address], registro.snippet.c_str(), registro.snippet.size()+1);
    address += registro.snippet.size()+1;

    // Atualiza o header do bloco
    bloco->header.espacoLivre = bloco->header.espacoLivre - registro.tamanhoRegistro - sizeof(int);
    bloco->header.numeroRegistros++;

    // Adiciona o novo offset do registro no vetor de offsets
    bloco->header.offsetRegistros.push_back(novoOffset);

    address = 0;

    // Copia as informações do header para o vetor de bytes do bloco
    memcpy(&bloco->blocoBytes[address], &bloco->header.espacoLivre, sizeof(int));
    address += sizeof(int);

    memcpy(&bloco->blocoBytes[address], &bloco->header.numeroRegistros, sizeof(int));
    address += sizeof(int);

    // Copia os offsets dos registros para o vetor de bytes do bloco
    for (int i = 0; i < bloco->header.numeroRegistros; i++) {
        memcpy(&bloco->blocoBytes[address], &bloco->header.offsetRegistros[i], sizeof(int));
        address += sizeof(int);
    }

    return true;
}

Registro * searchRegistroBloco(Bloco * bloco, int registroId){
    Registro *registro = new Registro();
    for(int offset : bloco->header.offsetRegistros){
        int pos = offset;
        memcpy(&registro->id, &bloco->blocoBytes[pos], sizeof(int));
        // cout<<"Comparando com o id: "<<registro->id<<endl;
        if(registro->id == registroId){
            pos += sizeof(int);

            memcpy(&registro->tamanhoRegistro, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            memcpy(&registro->ano, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            memcpy(&registro->citacoes, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            registro->atualizacao = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->atualizacao.size() + 1;

            registro->titulo = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->titulo.size() + 1;

            registro->autores = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->autores.size() + 1;

            registro->snippet = string((char *)&bloco->blocoBytes[pos]);

            return registro;
        }
    }

    return NULL;
}

#endif
#ifndef BUCKET_HPP
#define BUCKET_HPP

#include "../Constantes/cte.hpp"
#include "../Bloco/bloco.hpp"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

typedef struct Bucket{
    unsigned int blocos[BUCKET_SIZE];
} Bucket;

//Função que cria os Buckets e os insere vazios no arquivo
Bucket * createBucket(ofstream &binDataFile){
    Bucket* bucket = new Bucket();
    char buffer[BLOCO_SIZE];
    Bloco bloco = createBloco();
    for(int i = 0; i < BUCKET_SIZE; i++){
        bucket->blocos[i] = i;
        memcpy(buffer, &bloco.header, sizeof(HeaderBlock));
        memcpy(buffer + sizeof(HeaderBlock), bloco.blocoBytes, BLOCO_SIZE - sizeof(HeaderBlock));
        // memcpy(buffer, bloco.blocoBytes, BLOCO_SIZE);
        //Escreve o bloco no arquivo
        binDataFile.write(buffer, BLOCO_SIZE);
        // binDataFile.write(reinterpret_cast<char*>(bloco.blocoBytes), BLOCO_SIZE);
    }
    return bucket;
}

Bloco * loadBloco(int blockAddress, ifstream &binDataFile){
    char buffer[BLOCO_SIZE];
    Bloco *bloco = new Bloco();
    binDataFile.seekg(blockAddress);
    binDataFile.read(reinterpret_cast<char*>(buffer), BLOCO_SIZE);
    *bloco = extrairHeader(buffer);
    return bloco;
}

bool writeRegistroBucket(int blockAddress, Bloco *bloco, Registro registro, ofstream &dataFileWrite){
    insertRegistroBloco(bloco, registro); //Dar uma olhada
    char buffer[BLOCO_SIZE];
    memcpy(buffer, bloco->blocoBytes, BLOCO_SIZE);

    dataFileWrite.seekp(blockAddress);
    dataFileWrite.write(reinterpret_cast<char*>(buffer), BLOCO_SIZE);
    return true;
}

#endif
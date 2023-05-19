#ifndef BUCKET_HPP
#define BUCKET_HPP

#include "../Bloco/bloco.hpp"
#include <bits/stdc++.h>

using namespace std;

//Estrutura do Bucket, que é composto por um vetor de blocos
typedef struct Bucket{
    unsigned int blocos[BUCKET_SIZE];
} Bucket;

//Função que cria os Buckets e os insere vazios no arquivo
Bucket * createBucket(ofstream &binDataFile){
    Bucket* bucket = new Bucket();

    //Utilizamos um buffer para facilitar a escrita no arquivo
    char buffer[BLOCO_SIZE];

    //Cria um bloco vazio
    Bloco bloco = createBloco();

    //Iteramos em cada bloco do Bucket
    for(int i = 0; i < BUCKET_SIZE; i++){
        bucket->blocos[i] = i;

        //Copiamos o Header do bloco para o buffer
        memcpy(buffer, &bloco.header, sizeof(HeaderBlock));
        //Copiamos os dados do bloco para o buffer
        memcpy(buffer + sizeof(HeaderBlock), bloco.blocoBytes, BLOCO_SIZE - sizeof(HeaderBlock));
        
        //Escrevemos o buffer no arquivo
        binDataFile.write(buffer, BLOCO_SIZE);
    }
    return bucket;
}

//Função que carrega o bloco para a memória RAM e o retorna
Bloco * loadBloco(int blockAddress, ifstream &binDataFile){
    char buffer[BLOCO_SIZE];

    //Cria um novo Bloco
    Bloco *bloco = new Bloco();

    //Ajusta o ponteiro do arquivo para a posição inicial do bloco que queremos
    binDataFile.seekg(blockAddress);
    //Lê o bloco do arquivo e o coloca no buffer
    binDataFile.read(reinterpret_cast<char*>(buffer), BLOCO_SIZE);

    //Extrai o Header do buffer e o coloca no bloco
    *bloco = extrairHeader(buffer);
    return bloco;
}

//Escreve o Registro no Bucket relativo a ele, segundo a Hash
bool writeRegistroBucket(int blockAddress, Bloco *bloco, Registro registro, ofstream &dataFileWrite){

    //Insere o bloco no registro
    insertRegistroBloco(bloco, registro);

    //Ajusta o ponteiro do arquivo para a posição inicial do bloco que queremos
    dataFileWrite.seekp(blockAddress);
    //Escreve o bloco no arquivo
    dataFileWrite.write(reinterpret_cast<char*>(bloco->blocoBytes), BLOCO_SIZE);
    return true;
}

#endif
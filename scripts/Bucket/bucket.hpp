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
Bucket createBucket(ofstream &arquivo){
    Bucket bucket;
    char buffer[BLOCO_SIZE];
    Bloco bloco = createBloco();
    for(int i = 0; i < BUCKET_SIZE; i++){
        bucket.blocos[i] = i;
        memcpy(buffer, &bloco.header, sizeof(HeaderBlock));
        memcpy(buffer + sizeof(HeaderBlock), bloco.blocoBytes, BLOCO_SIZE - sizeof(HeaderBlock));
        //Escreve o bloco no arquivo
        arquivo.write(buffer, BLOCO_SIZE);
    }
    return bucket;
}

#endif
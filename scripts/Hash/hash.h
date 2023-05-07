#ifndef HASH_H
#define HASH_H

#include "../Constantes/cte.h"
#include "../Bucket/bucket.h"
#include "../Bloco/bloco.h"
#include "../Registro/registro.h"
#include <bits/stdc++.h>
#include <cstdint>
using namespace std;


//TODO Teste para encontrar os parâmetros dos buckets

typedef struct Hash{
  Bucket table[hash_size];
} Hash;

Hash createHash(){
  Hash hash;
  for(int i = 0; i < hash_size; i++){
    hash.table[i] = createBucket();
  }
  return hash;
}


int hashFunction(int key) {
    uint32_t ukey = static_cast<uint32_t>(key);

    ukey ^= ukey >> 16;
    ukey *= 0x85ebca6b;
    ukey ^= ukey >> 13;
    ukey *= 0xc2b2ae35;
    ukey ^= ukey >> 16;

    int result = static_cast<int>(ukey % hash_size);
    return result;
}



#endif
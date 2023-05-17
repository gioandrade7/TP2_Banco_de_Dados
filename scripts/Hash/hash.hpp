#ifndef HASH_HPP
#define HASH_HPP

#include "../Constantes/cte.hpp"
#include "../Bucket/bucket.hpp"
#include "../Bloco/bloco.hpp"
#include "../Registro/registro.hpp"
#include <bits/stdc++.h>
#include <cstdint>
using namespace std;


//TODO Teste para encontrar os parâmetros dos buckets

typedef struct HashTable{
  Bucket table[HASH_SIZE];
} HashTable;

HashTable createHash(ofstream &file){
  HashTable hashTable;
  for(int i = 0; i < HASH_SIZE; i++){
    hashTable.table[i] = createBucket(file);
  }
  return hashTable;
}

int hashFunction(int key) {
    uint32_t ukey = static_cast<uint32_t>(key);

    ukey ^= ukey >> 16;
    ukey *= 0x85ebca6b;
    ukey ^= ukey >> 13;
    ukey *= 0xc2b2ae35;
    ukey ^= ukey >> 16;

    int result = static_cast<int>(ukey % HASH_SIZE);
    return result;
}



#endif
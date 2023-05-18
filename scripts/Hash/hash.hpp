#ifndef HASH_HPP
#define HASH_HPP

#include "../Constantes/cte.hpp"
#include "../Bucket/bucket.hpp"
#include "../Bloco/bloco.hpp"
#include "../Registro/registro.hpp"
#include <cstdint>
using namespace std;


//TODO Teste para encontrar os parâmetros dos buckets

typedef struct HashTable{
  Bucket *table[HASH_SIZE];
} HashTable;

HashTable* createHash(ofstream &binDataFile){
  HashTable *hashTable = new HashTable();
  for(int i = 0; i < HASH_SIZE; i++){
    hashTable->table[i] = createBucket(binDataFile);
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

bool insertRegistroHashTable(Registro registro, ofstream &dataFileWrite, ifstream &dataFileRead){
// bool insertRegistroHashTable(HashTable *hashtable, Registro registro, ofstream &dataFileWrite, ifstream &dataFileRead){
  int key = hashFunction(registro.id);

  // Bucket *bucket = hashtable->table[key];
  // Bloco *bloco = NULL;

  for(int blocoId = 0; blocoId < BUCKET_SIZE; blocoId++){
  // for(int blocoId : bucket->blocos){
    int blockAddress = (blocoId * BLOCO_SIZE) + (key * BUCKET_SIZE * BLOCO_SIZE);
    Bloco *bloco = loadBloco(blockAddress, dataFileRead);
    // cout << endl << endl << "Bloco " << blocoId << endl <<"tamanho do registro: " << registro.tamanhoRegistro<<endl<<endl;
    // printBloco(bloco);
    if(bloco->header.espacoLivre >= (registro.tamanhoRegistro + sizeof(int))){
      //Inserir na árvore 1
      //Inserir na árvore 2
      //Inserir no bloco
      writeRegistroBucket(blockAddress, bloco, registro, dataFileWrite);
      // printBloco(bloco);
      delete bloco;
      return true;
    }
    delete bloco; // Liberando o bloco antes de carregar o próximo
    bloco = NULL;
  }
  // delete bloco;
  return false;
}

Registro * searchRegistroById(int registroId, ifstream &dataFileRead){
  int key = hashFunction(registroId);
  int blockAddress;
  Bloco *bloco = NULL;
  Registro *registro = NULL;

  for(int blocoId = 0; blocoId < BUCKET_SIZE; blocoId++){
    blockAddress = (blocoId * BLOCO_SIZE) + (key * BUCKET_SIZE * BLOCO_SIZE);
    bloco = loadBloco(blockAddress, dataFileRead);

    // cout << endl <<endl<< "Procurando no Bloco " << blocoId << endl;
    // printBloco(bloco);
    if(bloco->header.numeroRegistros == 0) {
      delete bloco;
      return NULL;
    }

    registro = searchRegistroBloco(bloco, registroId);
    
    if(registro != NULL){
      cout << "Foi percorrido " << blocoId+1 << " blocos para encontrar o Registro!" << endl;
      return registro;
    }
  }
  delete bloco;
  return NULL;
}

#endif
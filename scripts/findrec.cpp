#include <iostream>
#include <fstream>
#include <cstring>
#include "Hash/hash.hpp"

using namespace std;

int main(int argc, char const **argv){
    if(argc != 2){
        cout << "Usage: ./findrec <id>" << endl;
        return 0;
    }

    ifstream dataFileRead("Arquivos/dataFile.bin", ios::binary | ios::in);

    Registro* registro_busca = searchRegistroById(atoi(argv[1]), dataFileRead);
    if(registro_busca != NULL) {
        printRegistro(*registro_busca);
    } else {
        cout << "Registro não encontrado!" << endl;
    }

    //Fechamento do arquivo de dados organizado por hashing
    dataFileRead.close();

    return 0;
}
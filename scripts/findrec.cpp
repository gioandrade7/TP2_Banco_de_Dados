#include "Hash/hash.hpp"

using namespace std;

int main(int argc, char const **argv){
    //Conferindo os argumentos
    if(argc != 2){
        cout << "Forma correta: ./findrec <id>" << endl;
        return 0;
    }

    //Abrindo o arquivo de dados
    ifstream dataFileRead("Arquivos/dataFile.bin", ios::binary | ios::in);

    //Buscando o registro
    Registro* registro_busca = searchRegistroById(atoi(argv[1]), dataFileRead);

    //Imprimindo o registro
    if(registro_busca != NULL) {
        printRegistro(*registro_busca);
    } else {
        cout << "Registro não encontrado!" << endl;
    }

    //Fechando o arquivo de dados
    dataFileRead.close();

    return 0;
}
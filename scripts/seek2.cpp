#include "Hash/hash.hpp"

using namespace std;

int main(int argc, char const **argv){
    //Conferindo os argumentos
    if(argc != 2){
        cout << "Forma correta: ./seek2 <titulo>" << endl;
        return 0;
    }

    //Abrindo o arquivo de dados
    FILE *arq;
    arq = fopen("Arquivos/indexFile2.bin", "rb+");

    //Conferindo se o arquivo foi aberto corretamente
    if(arq == NULL){
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    imprime_arvore(hashStringFunction(argv[1]), arq);

    fclose(arq);

    return 0;
}
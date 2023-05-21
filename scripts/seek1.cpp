#include "B_Plus_Tree/bpt.hpp"

using namespace std;

int main(int argc, char const **argv){
    //Conferindo os argumentos
    if(argc != 2){
        cout << "Forma correta: ./seek1 <id>" << endl;
        return 0;
    }

    //Abrindo o arquivo de dados
    FILE *arq;
    arq = fopen("Arquivos/indexFile1.bin", "rb+");

    //Conferindo se o arquivo foi aberto corretamente
    if(arq == NULL){
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    imprime_arvore(atoi(argv[1]), arq);

    fclose(arq);

    return 0;
}
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
    
    //Abrindo o arquivo de dados
    ifstream dataFileRead("Arquivos/dataFile.bin", ios::binary | ios::in);

    if(!dataFileRead.is_open()){
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    unsigned int acessos = 0;
    unsigned int address = search_key(atoi(argv[1]), 0, &acessos, arq);

    bool flag = false;

    if(address == -1){
        cout << "Registro não encontrado!" << endl;
        return 0;
    }
    else{

        Bloco *bloco = loadBloco(address, dataFileRead);

        Registro *registro = new Registro();

        for(int offset : bloco->header.offsetRegistros){
            int pos = offset;

            memcpy(&registro->id, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            memcpy(&registro->tamanhoRegistro, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            memcpy(&registro->ano, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            memcpy(&registro->citacoes, &bloco->blocoBytes[pos], sizeof(int));
            pos += sizeof(int);

            registro->atualizacao = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->atualizacao.size() + 1;

            registro->titulo = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->titulo.size() + 1;

            registro->autores = string((char *)&bloco->blocoBytes[pos]);
            pos += registro->autores.size() + 1;

            registro->snippet = string((char *)&bloco->blocoBytes[pos]);

            if(registro->titulo == argv[1]){
                cout << "Registro encontrado!" << endl;
                cout << "Número de acessos: " << acessos << endl;
                flag = true;
                printRegistro(*registro);

                break;
            }
        }

        if(!flag){
            cout << "Registro não encontrado!" << endl;
        }
    }

    //Fechando o arquivo de dados
    dataFileRead.close();
    fclose(arq);
    return 0;
}
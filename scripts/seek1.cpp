#include "Hash/hash.hpp"

using namespace std;

int main(int argc, char const **argv)
{
    // Conferindo os argumentos
    if (argc != 2)
    {
        cout << "Forma correta: ./seek1 <id>" << endl;
        return 0;
    }

    // Abrindo o arquivo de dados
    FILE *arq;
    arq = fopen("indexFile1.bin", "rb+");

    // Conferindo se o arquivo foi aberto corretamente
    if (arq == NULL)
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    // Abrindo o arquivo de dados
    ifstream dataFileRead("dataFile.bin", ios::binary | ios::in);

    if (!dataFileRead.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    unsigned int acessos = 0;
    unsigned int address = search_key(atoi(argv[1]), 0, &acessos, arq);

    if (address == -1)
    {
        cout << "Registro não encontrado!" << endl;
        return 0;
    }
    else
    {
        cout << "Registro encontrado!" << endl;
        cout << "Número de acessos: " << acessos << endl;

        Bloco *bloco = loadBloco(address, dataFileRead);

        Registro *registro = searchRegistroBloco(bloco, atoi(argv[1]));

        printRegistro(*registro);
    }

    // Fechando o arquivo de dados
    dataFileRead.close();
    fclose(arq);

    return 0;
}
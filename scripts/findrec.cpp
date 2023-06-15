#include "Hash/hash.hpp"

using namespace std;

int main(int argc, char const **argv)
{
    // Conferindo os argumentos
    if (argc != 2)
    {
        cout << "Forma correta: ./findrec <id>" << endl;
        return 0;
    }

    // Abrindo o arquivo de dados
    ifstream dataFileRead("dataFile.bin", ios::binary | ios::in);

    if (!dataFileRead.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 0;
    }

    // Buscando o registro
    Registro *registro = searchRegistroById(atoi(argv[1]), dataFileRead);

    // Imprimindo o registro
    if (registro != NULL)
    {
        printRegistro(*registro);
    }
    else
    {
        cout << "Registro não encontrado!" << endl;
    }

    // Fechando o arquivo de dados
    dataFileRead.close();

    return 0;
}
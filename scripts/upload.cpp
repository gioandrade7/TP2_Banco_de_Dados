#include "./Hash/hash.hpp"

using namespace std;

// Função que retorna o próximo campo do arquivo de entrada (campo do registro), lendo ele até o fim ou até encontrar o padrão caracter por caracter
bool get_next_field(FILE *arquivo, char field[], string pattern){
    unsigned int pos = 0;
    char c;
    
    while (pos < 1029) {
        c = getc(arquivo);
        if (c == EOF) return true;
        if(c == 0 || (c >= 7 && c <= 13 ) || (c >= 32 && c <=126)){
            if(pos == 0){
                if(c == ';'){
                    field[0] = 'N';
                    field[1] = 'U';
                    field[2] = 'L';
                    field[3] = 'L';
                    field[4] = '\0';                  
                    return false;
                }
            }
            field[pos] = c;
            if(pos > 0){
                if(field[0] == 'N' && field[1] == 'U' && field[2] == 'L' && field[3] == 'L'){
                    if(field[pos] == 59 ){
                        field[pos] = '\0';
                        return false;
                    }   
                    else if(field[pos-1] == 13 && field[pos] == 10){
                        field[pos-1] = '\0';
                        return false;
                    }
                }
                if ((field[pos-1] == pattern[0]) && (field[pos] == pattern[1])) {
                    if(pattern[0] == 34){field[pos-1] = '\0';}
                    else{field[pos-2] = '\0';}
                    for(int i = 1; i < pos; i++){
                        field[i-1] = field[i];
                    }
                    return false;
                }
            }
            pos++;
        }
    }
    return false;
}

int main(int argc, char *argv[]){

    FILE *arquivo;
    char pattern[2], field[1030];

    // Abertura do arquivo de entrada
    arquivo = fopen(argv[1], "r");
    
    //Conferindo os argumentos
    if(argc != 2){
        cout << "Usage: ./upload <filePath>" << endl;
        return 0;
    }

    //Conferindo se o arquivo foi aberto corretamente
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo de entrada!\n";
        return -1;
    }

    // Caminho do arquivo de dados que vai ser organizado por Hash
    string dataFilePath = "Arquivos/dataFile.bin";

    // Criando o arquivo de dados em binário que vai ser organizado por hash
    ofstream dataFileWrite(dataFilePath, ios::binary | ios::out);
    if (!dataFileWrite) {
        cerr << "Erro ao criar o arquivo de dados en binário!" << endl;
        return 1;
    }


    // Criação da tabela hash 
    cout << "Criando a Tabela Hash..." << endl;
    HashTable* hashTable = createHash(dataFileWrite);
    cout << "Tabela hash criada com sucesso!" << endl;

    //Deletando a Tabela Hash da memória RAM, pois usaremos o arquivo de dados organizado por hash em disco
    delete hashTable;

    //Abertura do arquivo de dados organizado por hashing
    ifstream dataFileRead(dataFilePath, ios::binary | ios::in);
    if(!dataFileRead){
        cerr << "Erro ao abrir o arquivo de dados!" << endl;
        return 1;
    }


    cout << "Iniciando a inserção dos registros no arquivo de dados e criação dos índices..." << endl;

    //Fazendo a leitura e inserção dos registros
    while (true) {
        int id;
        int ano;
        int citacoes;
        string atualizacao; //Tamanho fixo
        string titulo;
        string autores;
        string snippet;

        pattern[0] = 34;
        pattern[1] = 59;
        
        if (get_next_field(arquivo, field, pattern)) break;
        id = stoi(field);

        if (get_next_field(arquivo, field, pattern)) break;
        titulo = field;

        if (get_next_field(arquivo, field, pattern)) break;
        ano = stoi(field);

        if (get_next_field(arquivo, field, pattern)) break;
        autores = field;
        
        if (get_next_field(arquivo, field, pattern)) break;
        citacoes = stoi(field);
        
        if (get_next_field(arquivo, field, pattern)) break;
        atualizacao = field;
        
        pattern[0] = 13;
        pattern[1] = 10;
        if (get_next_field(arquivo, field, pattern)) break;
        snippet = field;

        //Criando o registro
        Registro registro = createRegistro( id,  ano,  citacoes, atualizacao, titulo, autores, snippet);

        //Inserindo o registro no arquivo de dados e criando os índices
        insertRegistroHashTable(registro, dataFileWrite, dataFileRead);
    }
    
    cout << "Inserção dos registros no arquivo de dados e criação dos índices concluída!" << endl << endl;
    
    //Fechando os arquivos
    dataFileWrite.close();
    dataFileRead.close();
    fclose(arquivo);
    return 0;
}
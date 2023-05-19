#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>

using namespace std;

//Estrutura do Registro
typedef struct Registro{
    int id;

    //Header do Registro
    int tamanhoRegistro;
    //Dados do Registro
    
    int ano;
    int citacoes;
    string atualizacao; //Tamanho fixo
    string titulo;
    string autores;
    string snippet;
} Registro;

//Calcula o tamanho do registro e atualiza o campo tamanhoRegistro
void sizeRegistro(Registro *registro){
    int size = 0;
    size += sizeof(int); //id
    size += sizeof(int); //tamanhoRegistro
    size += sizeof(int); //ano
    size += sizeof(int); //citacoes
    //Nas strings somamos o tamanho da string + 1 (para o \0)
    size += registro->atualizacao.size()+1;
    size += registro->titulo.size()+1;
    size += registro->autores.size()+1;
    size += registro->snippet.size()+1;
    registro->tamanhoRegistro = size;
}

//Imprime o Registro em um Formato específico
void printRegistro(Registro registro){
    cout << endl;
    cout << "##############################################" << endl;
    cout << "ID: " << registro.id << endl;
    // cout << "Tamanho do Registro: " << registro.tamanhoRegistro << endl;
    cout << "Ano: " << registro.ano << endl;
    cout << "Citacoes: " << registro.citacoes << endl;
    cout << "Atualizacao: " << registro.atualizacao << endl;
    cout << "Titulo: " << registro.titulo << endl;
    cout << "Autores: " << registro.autores << endl;
    cout << "Snippet: " << registro.snippet << endl;
    cout << "##############################################" << endl << endl;
}

//Cria o Registro
Registro createRegistro(int id = 0, int ano = 0, int citacoes = 0, string atualizacao = "NULL", string titulo = "NULL", string autores = "NULL", string snippet = "NULL"){
    Registro registro;
    registro.id = id;
    registro.ano = ano;
    registro.citacoes = citacoes;
    registro.atualizacao = atualizacao;
    registro.titulo = titulo;
    registro.autores = autores;
    registro.snippet = snippet;
    sizeRegistro(&registro); //Atualiza tamanhoRegistro e Offsets
    return registro;
}

#endif
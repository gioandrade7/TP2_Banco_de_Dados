#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>

using namespace std;


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

void sizeRegistro(Registro *registro){
    int size = 0;
    size += sizeof(int); //id
    size += sizeof(int); //tamanhoRegistro
    size += sizeof(int); //ano
    size += sizeof(int); //citacoes
    size += registro->atualizacao.size()+1;
    size += registro->titulo.size()+1;
    size += registro->autores.size()+1;
    size += registro->snippet.size()+1;
    registro->tamanhoRegistro = size;
}

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

//Create Registro
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
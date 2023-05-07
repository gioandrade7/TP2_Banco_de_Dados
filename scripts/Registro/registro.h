#ifndef REGISTRO_H
#define REGISTRO_H

#include <bits/stdc++.h>
#include <string>
using namespace std;


typedef struct Registro{
    int id;

    //Header do Registro
    int tamanhoRegistro;
    int offsetAutores;
    int offsetSnippet;
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
    size += sizeof(int); //offsetAutores
    size += sizeof(int); //offsetSnippet
    size += sizeof(int); //ano
    size += sizeof(int); //citacoes
    size += registro->atualizacao.size()+1;
    size += registro->titulo.size()+1;
    registro->offsetAutores = size;
    size += registro->autores.size()+1;
    registro->offsetSnippet = size;
    size += registro->snippet.size()+1;
    registro->tamanhoRegistro = size;
}

//Create Registro
Registro createRegistro(int id, int ano, int citacoes, string atualizacao, string titulo, string autores, string snippet){
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
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
    // string titulo;
    // string autores;
    // string snippet;
} Registro;

#endif
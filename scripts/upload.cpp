#include<iostream>
#include "./Registro/registro.hpp"
#include <cstring>

using namespace std;

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

    FILE *arquivo/*, *arq*/;
    char char_curr, char_prev;
    char pattern[2], field[1030];

    arquivo = fopen(argv[1], "r");
    //arq = fopen("arqTeste.txt", "wt");
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo de entrada!\n";
        return -1;
    }

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
        //cout << "ID: " << field << " || ";
       // fprintf(arq, "ID: %s || ", field);
       id = stoi(field);

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tTITULO: " << field << " || ";
        //fprintf(arq, "\tTITULO: %s || ", field);
        titulo = field;

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tANO: " << field << " || ";
        //fprintf(arq, "\tANO: %s || ", field);
        ano = stoi(field);

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tAUTORES: " << field << " || ";
        //fprintf(arq, "\tAUTORES: %s || ", field);
        autores = field;
        
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tCITACOES: " << field << " || ";
        //fprintf(arq, "\tCITACOES: %s || ", field);
        citacoes = stoi(field);
        
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tATUALIZACAO: " << field << " || ";
        //fprintf(arq, "\tATUALIZACAO: %s || ", field);
        atualizacao = field;
        
        pattern[0] = 13;
        pattern[1] = 10;
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tSNIPET: " << field << endl << endl;
        //fprintf(arq, "\tSNIPET: %s || ", field);
        snippet = field;

        Registro registro = createRegistro( id,  ano,  citacoes, atualizacao, titulo, autores, snippet);
        printRegistro(registro);
    }
    fclose(arquivo);
    return 0;
}
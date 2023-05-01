#include<iostream>

using namespace std;

unsigned int get_next_field(FILE *arquivo, char field[], string pattern) {
    unsigned int pos = 0;
    char c;
    
    while (true) {
        c = getc(arquivo);
        if (c == EOF) return 1;

        field[pos] = c;
        if (pos > 0) {
            if ((field[pos-1] == pattern[0]) && (field[pos] == pattern[1])) {
                field[pos] = '\0';
                return 0;
            }
        }
        pos++;
    }
}

int main(int argc, char *argv[]){

    FILE *arquivo;
    char char_curr, char_prev;
    char pattern[2], field[1030];

    arquivo = fopen("artigo.csv", "r");
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo de entrada!";
        return -1;
    }

    while (true) {
        pattern[0] = 34;
        pattern[1] = 59;
        
        if (get_next_field(arquivo, field, pattern)) break;
        cout << "ID: " << field << endl;

        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tTITULO: " << field << endl;

        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tANO: " << field << endl;

        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tAUTORES: " << field << endl;
        
        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tCITACOES: " << field << endl;
        
        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tATUALIZACAO: " << field << endl;
        
        pattern[0] = 13;
        pattern[1] = 10;
        if (get_next_field(arquivo, field, pattern)) break;
        cout << "\tSNIPET: " << field << endl << endl;

    }
    // fclose(arquivo);
    return 0;
}


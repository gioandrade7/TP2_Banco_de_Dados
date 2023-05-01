#include<iostream>

using namespace std;

char* get_next_field(FILE *arquivo, unsigned int field_size, string pattern) {
    char* field = (char *) malloc(field_size * sizeof(char)); 
    unsigned int pos = 1;
    
    // fgetc(arquivo); // consume o '"' inicial
    field[0] = (char) fgetc(arquivo);
    field[1] = (char) fgetc(arquivo);
    while (true) {
        if (pos > 1) {
            // cout << field[pos] << '(' << pos << ')' << ' ' << id[pos-1] << '(' << pos-1 << ')' << endl;
            if ((field[pos] == pattern[1]) && (field[pos-1] == pattern[0])) {
                field[pos] = '\0';
                break;
            }
        }
        field[++pos] = (char) fgetc(arquivo);
    }
    return field;
}

int main(int argc, char *argv[]){

    FILE *arquivo;
    char char_curr, char_prev;
    char pattern[2];

    arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        cerr << "Não foi possível abrir o arquivo de entrada!";
        return -1;
    }

    while (!feof(arquivo)) {

        pattern[0] = 34;
        pattern[1] = 59;
        
        char* id = get_next_field(arquivo, 10, pattern);
        char* titulo = get_next_field(arquivo, 302, pattern);
        char* ano = get_next_field(arquivo, 8, pattern);
        char* autores = get_next_field(arquivo, 152, pattern);
        char* citacoes = get_next_field(arquivo, 12, pattern);
        char* data_hora = get_next_field(arquivo, 21, pattern);
        pattern[0] = 13;
        pattern[1] = 10;
        char* snipet = get_next_field(arquivo, 1030, pattern);
        
        cout << "ID: " << id << endl;
        cout << "\tTITULO: " << titulo << endl;
        cout << "\tANO: " << ano << endl;
        cout << "\tAUTORES: " << autores << endl;
        cout << "\tCITACOES: " << citacoes << endl;
        cout << "\tATUALIZACAO: " << data_hora << endl;
        cout << "\tSNIPET: " << snipet << endl << endl;

        free(id);
        free(titulo);
        free(ano);
        free(autores);
        free(citacoes);
        free(data_hora);
        free(snipet);
        // break;
    }    
    fclose(arquivo);
    return 0;
}


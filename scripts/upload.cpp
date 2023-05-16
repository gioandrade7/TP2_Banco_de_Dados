#include<iostream>

using namespace std;

bool get_next_field(FILE *arquivo, char field[], string pattern){
    unsigned int pos = 0;
    char c;
    
    while (pos < 1029) {
        c = getc(arquivo);
        if (c == EOF) return true;

        if(pos == 0){
            if(c == pattern[1]){
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
            if ((field[pos-1] == pattern[0]) && (field[pos] == pattern[1])) {
                field[pos-1] = '\0';
                for(int i = 1; i < pos; i++){
                    field[i-1] = field[i];
                }
                return false;
            }
        }
        pos++;
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
        pattern[0] = 34;
        pattern[1] = 59;
        
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "ID: " << field << " || ";
        //fprintf(arq, "ID: %s || ", field);

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tTITULO: " << field << " || ";
        //fprintf(arq, "\tTITULO: %s || ", field);

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tANO: " << field << " || ";
        //fprintf(arq, "\tANO: %s || ", field);

        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tAUTORES: " << field << " || ";
        //fprintf(arq, "\tAUTORES: %s || ", field);
        
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tCITACOES: " << field << " || ";
        //fprintf(arq, "\tCITACOES: %s || ", field);
        
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tATUALIZACAO: " << field << " || ";
        //fprintf(arq, "\tATUALIZACAO: %s || ", field);
        
        pattern[0] = 13;
        pattern[1] = 10;
        if (get_next_field(arquivo, field, pattern)) break;
        //cout << "\tSNIPET: " << field << endl << endl;
        //fprintf(arq, "\tSNIPET: %s || \n", field);

    }
    fclose(arquivo);
   // fclose(arq);
    return 0;
}

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

int main(int argc, char *argv[]){

    ifstream fp;
    fp.open(argv[1]);
    if(fp.fail()){
        cerr << "Não foi possível abrir o arquivo" << endl;
        return 1;
    }

    while(fp.peek() != EOF){
        string line;
        string tmp;
        getline(fp, line, '\n');
        line.erase(line.end()-1);
        stringstream ss(line);
        while(getline(ss, tmp, ';')){
            tmp.erase(tmp.begin());
            tmp.erase(tmp.end() - 1);
            cout << tmp << endl;
        }
    }

    fp.close();
    return 0;
}

#include <iostream>
#include <sstream>
#include "Formule.hpp"
#include "Parsers.hpp"

using namespace std;

int main(int argc, char** argv) {
//*
    formule *une_formule;
    string lue;
    map<string,int> correspondance;
    
    if (argc >= 2) {
        lue = string(argv[1]);
        une_formule = litFormuleFichier(argv[1]);
    } else {
        stringbuf buffer;
        cin.get(buffer);
        lue = buffer.str();
        une_formule = litFormuleString(lue);
    }
    cout << "Analyse de " << lue << endl;
    cout << formule2string(une_formule) << endl;
    numerote(une_formule, correspondance);
//*/
return 0;
}

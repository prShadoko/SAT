#include <iostream>
#include <sstream>
#include "Formule.hpp"
#include "Parsers.hpp"
#include "Solveur.hpp"

using namespace std;

int main(int argc, char** argv) {
	// DÉCLARATION DES VARIABLES
    formule *une_formule;
    string lue;
    map<string, int> correspondance;
    forme_conjonctive fc;
    char *valeurs = NULL;
    
    // LECTURE DE LA FORMULE
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
    
    // NUMÉROTATION DES VARIABLES
    numerote(une_formule, correspondance);
    // TRADUCTION EN FORME CONJONCTIVE
	fc = trad_forme_conjonctive(une_formule, correspondance);
	// EXPLORATION DE L'ESPACE DE RECHERCHE
	valeurs = new char[correspondance.size()];
	cherche(fc, 0, valeurs);
	
	// LIBÉRATION DES ALLOCATIONS
	delete[] valeurs;
	detruire_formule(une_formule);
	
	return 0;
}

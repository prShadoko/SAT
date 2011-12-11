#include <iostream>
#include <sstream>
#include "Formule.hpp"
#include "Parsers.hpp"
#include "Solveur.hpp"

using namespace std;

int main(int argc, char** argv) {
	// DÉCLARATION DES VARIABLES
    formule *une_formule;
    formule *formule_simplifiee;
    string lue;
    map<string, unsigned int> correspondance;
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
	formule_simplifiee = simplifie_formule(une_formule);
	cout << "Formule simplifiée :" << endl << formule2string(formule_simplifiee) << endl;
	fc = trad_forme_conjonctive(formule_simplifiee, correspondance);
/* Affichage de la forme conjonctive
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	for(fc_it=fc.begin(); fc_it!=fc.end(); fc_it++) {
		for(cl_it=fc_it->begin(); cl_it!=fc_it->end(); cl_it++) {
			cout << *cl_it << "\t";
		}
		cout << endl;
	}
//*/
	// EXPLORATION DE L'ESPACE DE RECHERCHE
	valeurs = new char[correspondance.size()];
	if(cherche(fc, valeurs, correspondance.size())) {
		cout << "La formule est satisfiable" << endl;
		for(unsigned int i=0; i<correspondance.size(); i++) {
			cout << (char)(i + 97) << " = " << (valeurs[i] > 0 ? "Vrai" : "Faux") << endl;
		}
	} else {
		cout << "La formule n'est pas satisfiable" << endl;
	}
	
	// LIBÉRATION DES ALLOCATIONS
	delete[] valeurs;
	detruire_formule(une_formule);
	detruire_formule(formule_simplifiee);
	
	return 0;
}

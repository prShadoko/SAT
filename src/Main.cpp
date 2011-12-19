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
    unsigned int nb_var;
    string lue;
    map<string, unsigned int> correspondance;
    forme_conjonctive fc;
    short int *interpretation = NULL;
    index_clauses index;

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
    nb_var = correspondance.size();
    // TRADUCTION EN FORME CONJONCTIVE
	formule_simplifiee = simplifie_formule(une_formule);
	cout << "Formule simplifiée :" << endl << formule2string(formule_simplifiee) << endl;
	fc = trad_forme_conjonctive(formule_simplifiee, correspondance);
/* Affichage de la forme conjonctive
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	for(fc_it=fc.begin(); fc_it!=fc.end(); fc_it++) {
		cout << "[";
		for(cl_it=fc_it->begin(); cl_it!=fc_it->end(); cl_it++) {
			cout.width(5);
			cout << *cl_it;
		}
		cout << " ]" << endl;
	}
//*/
	// EXPLORATION DE L'ESPACE DE RECHERCHE
	interpretation = new short int[nb_var];
	for(unsigned int i=0; i<nb_var; i++) {
		interpretation[i] = 0;
	}

	index = indexeClauses(fc);

//	if(cherche1(fc, interpretation, nb_var)) {
//	if(cherche2(fc, interpretation, nb_var)) {
//	if(cherche3(fc, interpretation, nb_var, index)) {
	if(cherche4(fc, interpretation, nb_var, index)) {
		cout << "La formule est satisfiable" << endl;
		for(unsigned int i=1; i<=correspondance.size(); i++) {
			for(map<string, unsigned int>::const_iterator it=correspondance.begin(); it!=correspondance.end(); it++) {
				if(it->second == i) {
					cout.width(4);
					cout << left << it->first;
					break;
				}
			}
			cout << " = ";
			if(interpretation[i-1] > 0) {
				cout << "Vrai";
			} else if(interpretation[i-1] < 0) {
				cout << "Faux";
			} else {
				cout << "?";
			}
			cout << endl;
		}
	} else {
		cout << "La formule n'est pas satisfiable" << endl;
	}

	// LIBÉRATION DES ALLOCATIONS
	delete[] interpretation;
	detruire_formule(une_formule);
	detruire_formule(formule_simplifiee);

	return 0;
}

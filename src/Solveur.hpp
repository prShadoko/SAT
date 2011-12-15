#ifndef Solveur
#define Solveur

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include "Formule.hpp"

using namespace std;

typedef vector<int> clause;
typedef vector<clause> forme_conjonctive;
struct index_clauses {
	map< unsigned int, vector<clause*> > pos;
	map< unsigned int, vector<clause*> > neg;
};

extern void numerote(const formule* form, map<string, unsigned int> &correspondance);
extern formule* simplifie_formule(const formule *form, const bool negation = false);
extern forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, unsigned int> &correspondance);
extern short int clause_est_satisfaite(const clause &cl, const short int *interpretation);
extern short int forme_conj_est_satisfaite(const forme_conjonctive &fc, const short int *interpretation);
extern bool cherche1(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const unsigned int id_var = 1);
extern bool cherche2(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const unsigned int id_var = 1);
extern bool cherche3(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const index_clauses &index, const unsigned int id_var = 1);
extern index_clauses indexeClauses(forme_conjonctive &fc);
extern bool contientInsatisfaite(const unsigned int id_var, const short int *interpretation, const index_clauses &index);

#endif

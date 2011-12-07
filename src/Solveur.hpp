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

extern void numerote(const formule* form, map<string, unsigned int> &correspondance);
extern formule* simplifie_formule(const formule *form, const bool negation = false);
extern forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, unsigned int> &correspondance);
extern char clause_est_satisfaite(const clause &cl, const char *valeurs);
extern char forme_conj_est_satisfaite(const forme_conjonctive &fc, const char *valeurs);
extern bool cherche(const forme_conjonctive &fc, char *valeurs, const unsigned int nb_valeurs, const unsigned int id_var = 1);

#endif

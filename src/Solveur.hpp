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

extern void numerote(const formule* form, map<string, int> &correspondance);
extern forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, int> &correspondance);
extern forme_conjonctive trad_fc_et(const forme_conjonctive &fc1, const forme_conjonctive &fc2);
extern forme_conjonctive trad_fc_ou(const forme_conjonctive &fc1, const forme_conjonctive &fc2);
extern forme_conjonctive etape_non(const forme_conjonctive &fc, const clause &cl);
extern forme_conjonctive trad_fc_non(const forme_conjonctive &fc);
extern char clause_est_satisfaite(const clause &cl, const char *valeurs);

#endif

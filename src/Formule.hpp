#ifndef Formule
#define Formule

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

typedef vector<int> clause;
typedef vector<clause> forme_conjonctive;

enum operateur {
  o_variable,
  o_non,
  o_et,
  o_ou,
  o_implique,
  o_equivaut
};

struct formule {
  operateur op;
  union {
    string* nom;
    struct {
      formule* arg1;
      formule* arg2;
    };
    formule* arg;
  };
};



extern formule * var(string* nom);
extern formule * var(string nom);
extern formule * non(formule* arg);
extern formule * et(formule* arg1, formule* arg2);
extern formule * ou(formule* arg1, formule* arg2);
extern formule * impl(formule* arg1, formule* arg2);
extern formule * equiv(formule* arg1, formule* arg2);

extern string operateur2string(const operateur op);
extern string formule2string(const formule * f);
extern bool is_binary(const operateur);
extern int priority_level(const operateur);
extern operateur operateur_for_level(const int lvl);

extern void numerote(const formule* form, map<string,int> &correspondance);
extern forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, int> &correspondance);
extern forme_conjonctive trad_fc_et(const forme_conjonctive &fc1, const forme_conjonctive &fc2);
extern forme_conjonctive trad_fc_ou(const forme_conjonctive &fc1, const forme_conjonctive &fc2);
extern forme_conjonctive etape_non(const forme_conjonctive &fc, const clause &cl);
extern forme_conjonctive trad_fc_non(const forme_conjonctive &fc);

#define START_LVL 5
#endif


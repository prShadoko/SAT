#ifndef Formule
#define Formule

#include <iostream>
#include <string>

using namespace std;

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
//extern void detruire_formule(formule *form);
extern formule* conversion_implique(const formule *form);
extern formule* conversion_equivaut(const formule *form);
#define START_LVL 5
#endif


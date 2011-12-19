#ifndef Solveur
#define Solveur

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include "Formule.hpp"

using namespace std;

/**
 * Représente une clause, soit une disjonction de littéraux.
 * Les littéraux sont représentés par le numéro de la variable correspondante.
 * Le signe définit si le littéral est une variable (positif), ou la négation d'une variable (négatif).
 * @see forme_conjonctive
 * @see index_clauses
 * @see short int clause_est_satisfaite(const clause&, const short int*)
 * @see index_clauses indexeClauses(forme_conjonctive&)
 */
typedef vector<int> clause;

/**
 * Représente une forme conjonctive, soit une conjonction de clauses.
 * @see clause
 * @see forme_conjonctive trad_forme_conjonctive(const formule*, map<string, unsigned int>&)
 * @see short int forme_conj_est_satisfaite(const forme_conjonctive&, const short int*)
 * @see bool cherche1(const forme_conjonctive&, short int*, const unsigned int, const unsigned int)
 * @see bool cherche2(const forme_conjonctive&, short int*, const unsigned int, const unsigned int)
 * @see bool cherche3(const forme_conjonctive&, short int*, const unsigned int, const index_clauses&, const unsigned int)
 * @see bool cherche4(const forme_conjonctive&, short int*, const unsigned int, const index_clauses&, const unsigned int)
 * @see index_clauses indexeClauses(forme_conjonctive&);
 */
typedef vector<clause> forme_conjonctive;

/**
 * Permet l'indexation des clauses d'une forme conjonctive selon les littéraux qu'elles contiennent.
 * Les littéraux négatifs sont stockés dans neg, tandis que les positifs sont stockés dans pos.
 * Tous les indices sont positifs (=numéro de la variable correspondante).
 * @see clause
 * @see bool cherche3(const forme_conjonctive&, short int*, const unsigned int, const index_clauses&, const unsigned int)
 * @see bool cherche4(const forme_conjonctive&, short int*, const unsigned int, const index_clauses&, const unsigned int)
 * @see indexeClauses(forme_conjonctive&)
 * @see bool contientInsatisfaite(const unsigned int, const short int*, const index_clauses&)
 * @see bool propage(const unsigned int, short int*, const index_clauses&, vector<unsigned int>&)
 */
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
extern bool cherche4(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const index_clauses &index, const unsigned int id_var = 1);
extern index_clauses indexeClauses(forme_conjonctive &fc);
extern bool contientInsatisfaite(const unsigned int id_var, const short int *interpretation, const index_clauses &index);
extern bool propage(const unsigned int id_var, short int *interpretation, const index_clauses &index, vector<unsigned int> &deduites);

#endif

#include "Formule.hpp"


string operateur2string(const operateur op) {
    switch (op) {
        case o_variable:
            return "var";
        case o_non:
            return "~";
        case o_et:
            return "/\\";
        case o_ou:
            return "\\/";
        case o_implique:
            return "=>";
        case o_equivaut:
            return "<=>";
    }
}

string formule2string(const formule* f) {
    switch (f->op) {
        case o_variable:
            return *(f->nom);
        case o_non:
            return "~" + formule2string(f->arg);
        case o_et:
        case o_ou:
        case o_equivaut:
        case o_implique:
            return "(" + formule2string(f->arg1) + " " + operateur2string(f->op) + " " + formule2string(f->arg2) + ")";
    }
}

bool is_binary(const operateur op) {
    switch (op) {
        case o_et:
        case o_ou:
        case o_equivaut:
        case o_implique:
            return true;
        default:
            return false;
    }
}

int priority_level(const operateur op) {
    switch (op) {
        case o_variable:
            return -1;
        case o_non:
            return 1;
        case o_et:
            return 2;
        case o_ou:
            return 3;
        case o_implique:
            return 4;
        case o_equivaut:
            return 5;
    }
}

operateur operateur_for_level(const int lvl) {
    switch (lvl) {
        case 1:
            return o_non;
        case 2:
            return o_et;
        case 3:
            return o_ou;
        case 4:
            return o_implique;
        case 5:
            return o_equivaut;
        default:
            return o_variable;
    }
}

formule* mkbinop(operateur op, formule* arg1, formule* arg2) {
    formule *res = new formule();
    res->op = op;
    res->arg1 = arg1;
    res->arg2 = arg2;
    return res;
}

formule* var(string* nom) {
    formule *res = new formule();
    res->op = o_variable;
    res->nom = nom;
    return res;
}

formule* var(string nom)  {
    formule *res = new formule();
    res->op = o_variable;
    res->nom = new string(nom);
    return res;
}

formule* non(formule* arg) {
    formule *res = new formule();
    res->op = o_non;
    res->arg = arg;
    return res;
}

formule* et(formule* arg1, formule* arg2) {
    return mkbinop(o_et,arg1,arg2);
}

formule* ou(formule* arg1, formule* arg2) {
    return mkbinop(o_ou,arg1,arg2);
}

formule* impl(formule* arg1, formule* arg2) {
    return mkbinop(o_implique,arg1,arg2);
}

formule* equiv(formule* arg1, formule* arg2) {
    return mkbinop(o_equivaut,arg1,arg2);
}

void detruire_formule(formule *form) {
	//TODO: Désallouer toutes les allocations d'une formule et de ses sous-formules
}

formule* conversion_implique(const formule *form) {
/*
	Précondition : form->op doit être o_implique
	Retour : une nouvelle formule formée des opérateurs de base (et, ou, non)
*/
	formule *formule_res = new formule();

	formule_res->arg1 = form->arg1;
	formule_res->arg2 = form->arg2;
	formule_res->arg1 = non(formule_res->arg1);
	formule_res->op = o_ou;

	return formule_res;
}

formule* conversion_equivaut(const formule *form) {
/*
	Précondition : form->op doit être o_equivaut
	Retour : une nouvelle formule formée des opérateurs de base (et, ou, non)
*/
	formule *formule_gauche = new formule();
	formule *formule_droite = new formule();
	formule *formule_res = new formule();

	formule_gauche->arg1 = form->arg1; 
	formule_gauche->arg2 = form->arg2;
	formule_gauche->op = o_implique;

	formule_droite->op = o_implique;
	formule_droite->arg1 = form->arg2; 
	formule_droite->arg2 = form->arg1;

	formule_res->op = o_et;
	formule_res->arg1 = formule_gauche;
	formule_res->arg2 = formule_droite;

	conversion_implique(formule_res->arg1);
	conversion_implique(formule_res->arg2);

	return formule_res;
}


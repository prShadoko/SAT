#include "Solveur.hpp"

// ================================================= NUMÉROTATION DES VARIABLES

/**
 * Numérote les variables d'une formule.
 * @param form La formule dont on veut numéroter les variables.
 * @param correspondance La table de correspondance.
 */
void numerote(const formule *form, map<string, unsigned int> &correspondance) {
	if (form->op == o_variable) {
		if(correspondance.count(*(form->nom)) == 1) {
			return;
		}
		int num = correspondance.size() + 1;
		correspondance[*(form->nom)] = num;
		cout << "variable " << *(form->nom) << " est le n°" << correspondance[*(form->nom)] << endl;
	} else {
        if(is_binary(form->op)) {
            numerote(form->arg1, correspondance);
			numerote(form->arg2, correspondance);
        } else {
            numerote(form->arg, correspondance);
        }
	}
}

// ============================================ TRADUCTION EN FORME CONJONCTIVE

/**
 * Simplifie une formule.
 * Cette fonction exprime les équivalences et implications de la formule en entrée avec les opérateurs ET, OU et NON.
 * Elle ramène également toutes les négations de la formule au niveau des littéraux.
 * @param form La formule à simplifier.
 * @param negation Indique si l'élément parent était un NON. Vaut false par défaut.
 * @return La formule simplifiée.
 */
formule* simplifie_formule(const formule *form, const bool negation) {
	formule *form_out = NULL;
	
	switch(form->op) {
		case o_variable:
		{
			if(negation) {
				form_out = non(var(*(form->nom)));
			} else {
				form_out = var(*(form->nom));
			}
			break;
		}
		
		case o_equivaut:
		{
			if(negation) {
				form_out = ou(	
								et(	simplifie_formule(form->arg1, true),
									simplifie_formule(form->arg2)),
								et(	simplifie_formule(form->arg2, true),
									simplifie_formule(form->arg1))
							);
			} else {
				form_out = et(
								ou(	simplifie_formule(form->arg1, true),
									simplifie_formule(form->arg2)),
								ou(	simplifie_formule(form->arg2, true),
									simplifie_formule(form->arg1))
							);
			}
			break;
		}
		
		case o_implique:
		{
			if(negation) {
				form_out = et(simplifie_formule(form->arg1), simplifie_formule(form->arg2, true));
			} else {
				form_out = ou(simplifie_formule(form->arg1, true), simplifie_formule(form->arg2));
			}
			break;
		}
		
		case o_non:
		{
			form_out = simplifie_formule(form->arg, !negation);
			break;
		}
		
		case o_ou:
		{
			if(negation) {
				form_out = et(simplifie_formule(form->arg1, true), simplifie_formule(form->arg2, true));
			} else {
				form_out = ou(simplifie_formule(form->arg1), simplifie_formule(form->arg2));
			}
			break;
		}
		
		case o_et:
		{
			if(negation) {
				form_out = ou(simplifie_formule(form->arg1, true), simplifie_formule(form->arg2, true));
			} else {
				form_out = et(simplifie_formule(form->arg1), simplifie_formule(form->arg2));
			}
			break;
		}
	}
	return form_out;
}

/** 
 * Traduit une formule en sa forme conjonctive.
 * @param form La formule simplifiée à transformer.
 * @param correspondance La liste des correspondances nom<->numéro des variables de la formule.
 * @return La forme conjonctive équivalente à la formule form.
 * @see formule
 * @see forme_conjonctive
 * @see simplifie_formule(const formule*, const bool)
 */
//TODO: correspondance.operator[] ajoute un élément à l'indice donné s'il n'y en a pas. À éviter.
forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, unsigned int> &correspondance) {
	forme_conjonctive fc_out, fc1, fc2;
	if(is_binary(form->op)) {
		fc1 = trad_forme_conjonctive(form->arg1, correspondance);
		fc2 = trad_forme_conjonctive(form->arg2, correspondance);
	}
	switch(form->op) {
		case o_variable:
		{
			clause cl;
			cl.push_back(correspondance[*(form->nom)]);
			fc_out.push_back(cl);
			break;
		}
		
		case o_non:
		{
			clause cl;
			cl.push_back(-correspondance[*(form->arg->nom)]);
			fc_out.push_back(cl);
			break;
		}
		
		case o_ou:
		{
			forme_conjonctive::const_iterator it1, it2;
			clause cl;

			for(it1=fc1.begin(); it1!=fc1.end(); it1++) {
				for(it2=fc2.begin(); it2!=fc2.end(); it2++) {
					cl.clear();
					set_union(it1->begin(), it1->end(), it2->begin(), it2->end(), insert_iterator<clause>(cl, cl.begin()));
					fc_out.push_back(cl);
				}
			}
			break;
		}
		
		case o_et:
		{
			set_union(fc1.begin(), fc1.end(), fc2.begin(), fc2.end(), insert_iterator<forme_conjonctive>(fc_out, fc_out.begin()));
			break;
		}
	}
	return fc_out;
}

// ======================================= EXPLORATION DE L'ESPACE DE RECHERCHE

/**
 * 
 * 
 */
char clause_est_satisfaite(const clause &cl, const char *valeurs) {
	bool indetermine = false;
	for(clause::const_iterator it=cl.begin(); it!=cl.end(); it++) {
		unsigned int indice = abs(*it) - 1;
		int litteral = *it * valeurs[indice];
		if(litteral > 0) {
			return 1;
		} else if(litteral == 0) {
			indetermine = true;
		}
	}
	return (indetermine ? 0 : -1);
}

/**
 * 
 * 
 */
char forme_conj_est_satisfaite(const forme_conjonctive &fc, const char *valeurs) {
	bool indetermine = false;
	for(forme_conjonctive::const_iterator it=fc.begin(); it!=fc.end(); it++) {
		char cl_satisfaite = clause_est_satisfaite(*it, valeurs);
		if(cl_satisfaite < 0) {
			return -1;
		} else if(cl_satisfaite == 0) {
			indetermine = true;
		}
	}
	return (indetermine ? 0 : 1);
}

/**
 * 
 * 
 */
bool cherche1(const forme_conjonctive &fc, char *valeurs, const unsigned int nb_valeurs, const unsigned int id_var) {
	const int indice = id_var - 1;
	if(indice >= nb_valeurs) {
		return (forme_conj_est_satisfaite(fc, valeurs) == 1);
	}
	valeurs[indice] = 1;
	if(cherche1(fc, valeurs, nb_valeurs, id_var+1)) {
		return true;
	} else {
		valeurs[indice] = -1;
		if(cherche1(fc, valeurs, nb_valeurs, id_var+1)) {
			return true;
		} else {
			valeurs[indice] = 0;
			return false;
		}
	}
}

/**
 * 
 * 
 */
bool cherche2(const forme_conjonctive &fc, char *valeurs, const unsigned int nb_valeurs, const unsigned int id_var) {
	const int indice = id_var - 1;
	
	for(valeurs[indice]=1; valeurs[indice]>=-1; valeurs[indice] -= 2) { // 1,-1
		if(forme_conj_est_satisfaite(fc, valeurs) == 1) {
			return true;
		}
		if(id_var < nb_valeurs) { // S'il reste des variables indéfinies
			if(cherche2(fc, valeurs, nb_valeurs, id_var + 1)) {
				return true;
			}
		}
	}
	
	valeurs[indice] = 0;
	return false;
}






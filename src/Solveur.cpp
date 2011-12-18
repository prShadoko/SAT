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
 * Évalue la valeur de vérité d'une clause selon une interprétation.
 * @param cl La clause à évaluer.
 * @param interpretation L'interprétation selon laquelle la clause sera évaluée.
 * @return -1 si la clause est évaluée à FAUX. 1 si elle est évaluée à VRAI. 0 si elle est indéterminée.
 */
short int clause_est_satisfaite(const clause &cl, const short int *interpretation) {
	bool indetermine = false;
	for(clause::const_iterator it=cl.begin(); it!=cl.end(); it++) {
		unsigned int indice = abs(*it) - 1;
		int litteral = *it * interpretation[indice];
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
 * @param fc
 * @param interpretation
 * @return
 */
short int forme_conj_est_satisfaite(const forme_conjonctive &fc, const short int *interpretation) {
	bool indetermine = false;
	for(forme_conjonctive::const_iterator it=fc.begin(); it!=fc.end(); it++) {
		short int cl_satisfaite = clause_est_satisfaite(*it, interpretation);
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
 * @param fc
 * @param interpretation
 * @param nb_var
 * @param id_var
 * @return
 */
bool cherche1(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const unsigned int id_var) {
	const int indice = id_var - 1;
	if(indice >= nb_var) {
		return (forme_conj_est_satisfaite(fc, interpretation) == 1);
	}
	interpretation[indice] = 1;
	if(cherche1(fc, interpretation, nb_var, id_var+1)) {
		return true;
	} else {
		interpretation[indice] = -1;
		if(cherche1(fc, interpretation, nb_var, id_var+1)) {
			return true;
		} else {
			interpretation[indice] = 0;
			return false;
		}
	}
}

/**
 *
 * @param fc
 * @param interpretation
 * @param nb_var
 * @param id_var
 * @return
 */
bool cherche2(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const unsigned int id_var) {
	const int indice = id_var - 1;

	for(interpretation[indice]=1; interpretation[indice]>=-1; interpretation[indice] -= 2) { // 1,-1
		if(forme_conj_est_satisfaite(fc, interpretation) == 1) {
			return true;
		}
		if(id_var < nb_var) { // S'il reste des variables indéfinies
			if(cherche2(fc, interpretation, nb_var, id_var + 1)) {
				return true;
			}
		}
	}

	interpretation[indice] = 0;
	return false;
}

/**
 *
 * @param fc
 * @param interpretation
 * @param nb_var
 * @param index
 * @param id_var
 * @return
 */
bool cherche3(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const index_clauses &index, const unsigned int id_var) {
//*
	const int indice = id_var - 1;
	for(interpretation[indice]=1; interpretation[indice]>=-1; interpretation[indice] -= 2) { // 1,-1
		if(!contientInsatisfaite(id_var, interpretation, index)) {
			if(id_var < nb_var) { // S'il reste des variables indéfinies
				if(cherche3(fc, interpretation, nb_var, index, id_var + 1)) {
					return true;
				}
			} else {
				return true;
			}
		}
	}

	interpretation[indice] = 0;
	return false;

}

bool cherche4(const forme_conjonctive &fc, short int *interpretation, const unsigned int nb_var, const index_clauses &index, const unsigned int id_var) {
//*
	const int indice = id_var - 1;
	vector<int> deduites;
	for(interpretation[indice]=1; interpretation[indice]>=-1; interpretation[indice] -= 2) { // 1,-1
		if(!propage(id_var, interpretation, index, deduites)) {
			if(id_var < nb_var) { // S'il reste des variables indéfinies
				if(cherche4(fc, interpretation, nb_var, index, id_var + 1)) {
					return true;
				}
			} else {
				return true;
			}
		}
	}

	interpretation[indice] = 0;
	return false;

}

/**
 * Indexe les clauses selon les littéraux qu'elles contiennent.
 * @param fc La forme conjonctive dont on indexe les clauses.
 * @return L'index des clauses.
 */
index_clauses indexeClauses(forme_conjonctive &fc) {
 	index_clauses index;
	for(forme_conjonctive::iterator it_fc=fc.begin(); it_fc!=fc.end(); it_fc++) {
		for(clause::iterator it_cl=it_fc->begin(); it_cl!=it_fc->end(); it_cl++) {
			if(*it_cl < 0) {
				index.neg[-(*it_cl)].push_back(&(*it_fc));
			} else {
				index.pos[*it_cl].push_back(&(*it_fc));
			}
		}
	}
	return index;
}

/**
 * Détermine si l'une des clauses de la formule est insatisfaite par une interprétation donnée.
 * @param id_var La variable par rapport à laquelle on évalue la satisfaction de la formule.
 * @param interpretation L'interprétation selon laquelle les clauses seront évaluées.
 * @param index L'index des clauses de la formule.
 * @return VRAI si l'une des clauses n'est pas satisfiable avec l'interprétation donnée. FAUX sinon.
 */
bool contientInsatisfaite(const unsigned int id_var, const short int *interpretation, const index_clauses &index) {

	unsigned int indice = id_var - 1;
	map< unsigned int, vector<clause*> >::const_iterator it_clauses;
	vector<clause*> clauses;
	if(interpretation[indice] == 0) {

		return false;
	}

	if(interpretation[indice] > 0) {

		if((it_clauses = index.neg.find(id_var)) == index.neg.end()) {
			return false;
		} else {
			clauses = it_clauses->second;
		}
	} else {
		if((it_clauses = index.pos.find(id_var)) == index.pos.end()) {
			return false;
		} else {
			clauses = it_clauses->second;
		}
	}

	for(vector<clause*>::const_iterator it=it_clauses->second.begin(); it!=it_clauses->second.end(); it++) {
		if(clause_est_satisfaite((**it), interpretation) == -1) {

			return true;
		}
	}

	return false;
}

bool propage(const unsigned int id_var, const short int *interpretation, const index_clauses &index, vector<int> &deduites) {

	unsigned int indice = id_var - 1;
	map< unsigned int, vector<clause*> >::const_iterator it_clauses;
	vector<clause*> clauses;
	vector<int>::iterator it_litt;
	int nb_litt, nb_litt_ind, nb_litt_neg, id_ind;
	
	if(interpretation[indice] == 0) {

		clauses = it_clauses->second;
	}

	if(interpretation[indice] > 0) {

		if((it_clauses = index.neg.find(id_var)) == index.neg.end()) {
			return false;
		} else {
			clauses = it_clauses->second;
		}
	} else {
		if((it_clauses = index.pos.find(id_var)) == index.pos.end()) {
			return false;
		} else {
			clauses = it_clauses->second;
		}
	}

	for(vector<clause*>::const_iterator it=it_clauses->second.begin(); it!=it_clauses->second.end(); it++) {
		for(it_litt=(*it)->begin(); it_litt!=(*it)->end(); it_litt++) {
			nb_litt++;
			if(interpretation[*it_litt-1] == -1) {
				nb_litt_neg++;
			}
			if(interpretation[*it_litt-1] == 0) {
				nb_litt_ind++;
				id_ind = *it_litt;
			}
		}
		if(nb_litt == nb_litt_neg+nb_litt_ind && nb_litt_ind == 1) {
			deduites.push_back(*it_litt);
			return propage(id_ind, interpretation, index, deduites);
		}
		if(clause_est_satisfaite((**it), interpretation) == -1) {

			return true;
		}
	}

	return false;
}

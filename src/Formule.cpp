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

//================================================================================ FONCTIONS IMPLÉMENTÉES CI-DESSOUS //

void numerote(const formule* form, map<string, int> &correspondance) {
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

forme_conjonctive trad_forme_conjonctive(const formule *form, map<string, int> &correspondance) {
	forme_conjonctive fc, fc1, fc2;
	//TODO: À la descente, utiliser une formule temporaire pour modifier les => et <=>
	if(is_binary(form->op)) {
		fc1 = trad_forme_conjonctive(form->arg1, correspondance);
		fc2 = trad_forme_conjonctive(form->arg2, correspondance);
	}
	
	switch(form->op) {
		case o_variable:
		{
			clause cl;
			cl.push_back(correspondance[*(form->nom)]);
			fc.push_back(cl);
			break;
		}
		case o_non:
		{
			fc = trad_forme_conjonctive(form->arg, correspondance);
			trad_fc_non(fc);
			break;
		}
		case o_et:
		{
			fc = trad_fc_et(fc1, fc2);
			break;
		}
		case o_ou:
		{
			fc = trad_fc_ou(fc1, fc2);
			break;
	  	}/*
	  	case o_implique:
	  		break;
  		
  		case o_equivaut:
  			break;
  		*/
	}
	return fc;
	
	
}

forme_conjonctive trad_fc_et(const forme_conjonctive &fc1, const forme_conjonctive &fc2) {
	forme_conjonctive fc;
	insert_iterator<forme_conjonctive> it_fcOut (fc, fc.begin());
	set_union(fc1.begin(), fc1.end(), fc2.begin(), fc2.end(), it_fcOut);
	return fc;
}

forme_conjonctive trad_fc_ou(const forme_conjonctive &fc1, const forme_conjonctive &fc2) {
	forme_conjonctive fc;
	forme_conjonctive::const_iterator it1, it2;
	clause cl;
	
	for(it1=fc1.begin(); it1!=fc1.end(); it1++) {
		for(it2=fc2.begin(); it2!=fc2.end(); it2++) {
			cl.clear();
			set_union(it1->begin(), it1->end(), it2->begin(), it2->end(), insert_iterator<clause>(cl, cl.begin()));
			fc.push_back(cl);
		}
	}
}

forme_conjonctive etape_non(const forme_conjonctive &fcIn, const clause &clIn) {
	clause::const_iterator it_clIn;
	forme_conjonctive::const_iterator it_fcIn;
	clause tmp_clIn, tmp_clOut;
	forme_conjonctive tmp_fc, res, tmp_res;
	
	for(it_clIn=clIn.begin(); it_clIn!=clIn.end(); it_clIn++) {
		tmp_clIn.push_back(-*it_clIn);
		for(it_fcIn=fcIn.begin(); it_fcIn!=fcIn.end(); it_fcIn++) {
			set_union(it_fcIn->begin(), it_fcIn->end(), tmp_clIn.begin(), tmp_clIn.end(), insert_iterator<clause>(tmp_clOut, tmp_clOut.begin()));
			tmp_fc.push_back(tmp_clOut);
			tmp_clOut.clear();
		}
		tmp_res = res;
		res.clear();
		set_union(tmp_res.begin(), tmp_res.end(), tmp_fc.begin(), tmp_fc.end(), insert_iterator<forme_conjonctive>(res, res.begin()));
		tmp_fc.clear();
		tmp_res.clear();
		tmp_clIn.clear();
	}
	return res;
}

forme_conjonctive trad_fc_non(const forme_conjonctive &fc) {
	forme_conjonctive res;
	forme_conjonctive::const_iterator it;
	res.push_back(clause());
	
	for(it=fc.begin(); it!=fc.end(); it++) {
		res = etape_non(res, *it);
	}
	
	return res;
}




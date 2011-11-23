/*-----TESTS-----*/

/*----- TEST trad_fc_non() -----
	forme_conjonctive fc;
	clause cl;
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	cl.push_back(-1);
	cl.push_back(-3);
	fc.push_back(cl);
	cl.clear();
	cl.push_back(-2);
	cl.push_back(-1);
	cl.push_back(-3);
	fc.push_back(cl);
	
	fc = trad_fc_non(fc);

	for(fc_it=fc.begin(); fc_it<fc.end(); fc_it++) {
		for(cl_it=fc_it->begin(); cl_it<fc_it->end(); cl_it++) {
			cout << *cl_it << "\t";
		}
		cout << endl;
	}
//*/
/*----- TEST etape_non() -----
	forme_conjonctive fc;
	clause cl;
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	cl.push_back(1);
	cl.push_back(3);
	cl.push_back(-7);
	
	fc.push_back(clause(1, -2));
	fc = etape_non(fc, cl);

	for(fc_it=fc.begin(); fc_it<fc.end(); fc_it++) {
		for(cl_it=fc_it->begin(); cl_it<fc_it->end(); cl_it++) {
			cout << *cl_it << " ";
		}
		cout << endl;
	}
//*/
/*----- TEST set_union() -----
	forme_conjonctive fc;
	clause clIn, clOut;
	insert_iterator<clause> it(clOut, clOut.begin());
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	
	
	fc.push_back(clause(1, 3));
	clIn.push_back(7);
	set_union(fc.begin()->begin(), fc.begin()->begin(), clIn.begin(), clIn.end(), it);
	
	for(fc_it=fc.begin(); fc_it!=fc.end(); fc_it++) {
		for(cl_it=fc_it->begin(); cl_it!=fc_it->end(); cl_it++) {
			cerr << *(cl_it) << endl;
		}
	}
//*/
/*----- TEST trad_fc_et() -----
	forme_conjonctive fc, fc1, fc2;
	forme_conjonctive::const_iterator fc_it;
	clause::const_iterator cl_it;
	
	fc1.push_back(clause(1, 3));
	fc2.push_back(clause(1, 4));
	fc = trad_fc_et(fc1, fc2);
	
	for(fc_it=fc.begin(); fc_it!=fc.end(); fc_it++) {
		for(cl_it=fc_it->begin(); cl_it!=fc_it->end(); cl_it++) {
			cerr << *(cl_it) << endl;
		}
	}
//*/

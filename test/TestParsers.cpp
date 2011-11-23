#include <UnitTest++.h>
#include "../src/Parsers.hpp"
#include <string>

TEST(ParseVariable)
{
  formule * f = litFormuleString("p");
  CHECK(f->op == o_variable);
  CHECK(f->nom->compare("p") == 0);
}

TEST(ParseFormule)
{
  formule * f1 = et(var("p"),var("q"));
  formule * f2 = litFormuleString("p /\\  q");
  string s1 = formule2string(f1);
  string s2 = formule2string(f2);
  CHECK(s1.compare(s2) == 0);
}

TEST(ParseFile)
{
  formule * f1 = et(var("p"),var("q"));
  formule * f2 = litFormuleFichier("test/formules/formule_1.txt");
  CHECK(f2 != NULL);
  string s1 = formule2string(f1);
  string s2 = formule2string(f2);
  CHECK(s1.compare(s2) == 0);
}

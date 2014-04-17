//
// conentropy.cpp
//
// replace attributes in concept lattice dot file by concept entropy
//
// author: bjkeller
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#include "concept.h"
#include "dotlattice.h"

using namespace std;

class entropy_writer : public concept_writer {
public:
  entropy_writer(size_t count) : attrcnt(count) {}
  void writeAttributes (ostream & os, const concept& con) const {
    double entropy = 0.0;
    if (!con.getAttributes().empty() && con.getAttributes().size() != attrcnt) {
      double p_con = con.getAttributes().size() / static_cast<double>(attrcnt);
      double log_p_con = log2(p_con);
      entropy = -1*(p_con*log_p_con);
    }
    os << "\\ncard=" << con.getAttributes().size() << ", ";
    os << "H=" << entropy;
  }
private:
  size_t attrcnt;
};

int main(int argc, char* argv[]) {
  lattice_map downMap;
  size_t attrcnt = 0;

  string line;
  getline(cin,line);
  while (cin) {
    size_t cnt = parseArc(downMap,line);
    if (cnt > attrcnt) {
      attrcnt = cnt;
    }
    getline(cin,line);
  }

  concept_writer* wrt = new entropy_writer(attrcnt);
  writeLattice(cout,downMap,wrt);
  delete wrt;
}

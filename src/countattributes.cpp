//
// countattributes.cpp
//
// replace attributes in concept lattice dot file, by count of attributes
//
// author: bjkeller
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.

#include <string>
#include <iostream>

#include "lattice.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  fca::concept_writer_ptr w(new fca::attrcount_writer(cout));
  fca::arc_writer_ptr v(new fca::arc_writer(w));

  cout << "digraph lattice { " << endl;

  fca::readDotLattice(cin,v);

  cout << "}" << endl;

}

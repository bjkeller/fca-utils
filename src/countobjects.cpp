//
// countobjects.cpp
//
// replace objects in concept lattice dot file, by count
//
// author: bjkeller
//  Copyright (c) 2014 Benjamin Keller. All rights reserved.

//TODO: merge with countattributes

#include <string>
#include <iostream>

#include "lattice.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  fca::concept_writer_ptr w(new fca::objcount_writer(cout));
  fca::arc_writer_ptr v(new fca::arc_writer(w));

  cout << "digraph lattice { " << endl;

  fca::readDotLattice(cin,v);

  cout << "}" << endl;

}

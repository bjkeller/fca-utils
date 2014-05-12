//
//  depchaintable.cpp
//
//  Modified dependence.cpp, Created by Benjamin Keller on 7/10/12.
//  Copyright (c) 2012, 2013 Benjamin J. Keller. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

#include <algorithm>

#include "concept.hpp"
#include "lattice.hpp"
#include "depchain.hpp"


int main(int argc, char* argv[]) {
  std::cout << "CAUTION: only works correctly if intent of top and extent of bot are empty" << std::endl;

  fca::lattice l; //downward map
  std::priority_queue<fca::concept> coatoms;
  fca::chain_map chains;

  fca::depchain_visitor* v = new fca::depchain_visitor(l,coatoms,chains);
  fca::readDotLattice(std::cin,v);
  delete v;

  fca::findAltChains(std::cout,coatoms,chains,l);

}

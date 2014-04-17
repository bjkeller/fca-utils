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
#include <queue>
#include <algorithm>

#include "concept.hpp"
#include "lattice.hpp"

class depchain_visitor : public fca::downlattice_visitor {
public:
  depchain_visitor(fca::lattice& l,std::priority_queue<fca::concept>& ca,std::map<fca::concept,std::string>& ch):
  downlattice_visitor(l), coatoms(ca), chains(ch) {}
  /* virtual */ void visitArc(const fca::concept&, const fca::concept&);
private:
  std::priority_queue<fca::concept>& coatoms;
  std::map<fca::concept,std::string>& chains;
};

//creates string representing concept as list of attributes followed by cardinality of objects
//e.g., "w0,w1",451
//
std::string conceptString(const fca::concept& c) {
  std::ostringstream result;

  result << "\"";

  std::set<std::string>::const_iterator a_i = c.getAttributes().begin();
  if (a_i != c.getAttributes().end()) {
    result << *a_i++;
    while (a_i != c.getAttributes().end()) {
      result << "," << *a_i;
      a_i++;
    }
  }

  result << "\",";
  result << c.getObjects().size();

  return result.str();
}

void depchain_visitor::visitArc(const fca::concept& super, const fca::concept& sub) {
  if (isTop(super)) { //if arc is top->coatom
    coatoms.push(sub);
    chains[sub] = conceptString(sub);
  } else {
    downlattice_visitor::visitArc(super,sub);
  }
}

class maxsub_visitor : public fca::arc_visitor {
public:
  maxsub_visitor() : maxsz(0), max() {}
  void visitArc(const fca::concept&, const fca::concept&);
  const std::set<fca::concept>& maxSet() const { return max; }
private:
  std::size_t maxsz;
  std::set<fca::concept> max;
};

void maxsub_visitor::visitArc(const fca::concept& super, const fca::concept& sub) {
  size_t sz = sub.getObjects().size();
  if (sz != 0 && sz >= maxsz) {
    if (sz > maxsz) {
      max = std::set<fca::concept>();
      maxsz = sz;
    }
    max.insert(sub);
  }
}

//finds alt chains from concepts in tovisit queue to atoms of downMap such that
// intent of included concepts is largest among possible choices
//
void findAltChains(std::ostream& os, std::priority_queue<fca::concept>& tovisit,
          std::map<fca::concept,std::string>& chain_strings, fca::lattice& downMap) {

  std::set<fca::concept> visited;

  while (!tovisit.empty()) {
    fca::concept next = tovisit.top();
    tovisit.pop();

    if (visited.find(next) == visited.end() && !isBot(next)) { //not visited and not bottom
      visited.insert(next); //mark visited

      std::string chain_to_next = "";
      std::map<fca::concept,std::string>::const_iterator ch_i = chain_strings.find(next);
      if (ch_i != chain_strings.end()) {
        chain_to_next = ch_i->second;
      } else {
        std::cerr << "badness: couldn't find chain for concept" << std::endl;
      }

      maxsub_visitor* mv = new maxsub_visitor();
      downMap.accept(next,mv);

      std::set<fca::concept> max = mv->maxSet();
      if (max.size() > 0) {
        std::set<fca::concept>::const_iterator s_i = max.begin();
        while (s_i != max.end()) {
          if (chain_strings.find(*s_i) == chain_strings.end()) {
            chain_strings[*s_i] = chain_to_next + "," + conceptString(*s_i);
          }
          tovisit.push(*s_i);
          s_i++;
        }
        chain_strings.erase(ch_i);
      }

      delete mv;
    }
  }
}

int main(int argc, char* argv[]) {
  std::cout << "CAUTION: only works correctly if intent of top and extent of bot are empty" << std::endl;

  fca::lattice l; //downward map
  std::priority_queue<fca::concept> coatoms;
  std::map<fca::concept,std::string> chains;

  depchain_visitor* v = new depchain_visitor(l,coatoms,chains);
  readDotLattice(std::cin,v);
  delete v;

  findAltChains(std::cout,coatoms,chains,l);

}

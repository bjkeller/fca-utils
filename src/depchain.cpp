#include "depchain.hpp"

namespace fca {
/*
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
*/

void depchain_visitor::visitArc(const fca::concept& super, const fca::concept& sub) {
  if (isTop(super)) { //if arc is top->coatom
    coatoms.push(sub);
    chains.add(sub); //start a new chain
  } else {
    downarc_visitor::visitArc(super,sub);
  }
}

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
          chain_map& chains, fca::lattice& downMap) {

  std::set<fca::concept> visited;

  while (!tovisit.empty()) {
    fca::concept next = tovisit.top();
    tovisit.pop();

    if (visited.find(next) == visited.end() && !isBot(next)) { //not visited and not bottom
      visited.insert(next); //mark visited

      maxsub_visitor* mv = new maxsub_visitor();
      downMap.accept(next,mv);

      std::set<fca::concept> max = mv->maxSet();
      if (max.size() > 0) {
        chains.push(next,max);
        std::set<fca::concept>::const_iterator s_i = max.begin();
        while (s_i != max.end()) {
          tovisit.push(*s_i);
          s_i++;
        }
      }

      delete mv;
    }
  }
}

}; //namespace fca

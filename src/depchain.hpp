#ifndef _depchain_h
#define _depchain_h

#include <queue>

#include "lattice.hpp"
#include "chain_map.hpp"

namespace fca {

class depchain_visitor : public fca::downarc_visitor {
public:
  depchain_visitor(fca::lattice& l,std::priority_queue<fca::concept>& ca,chain_map& ch):
  downarc_visitor(l), coatoms(ca), chains(ch) {}
  /* virtual */ void visitArc(const fca::concept&, const fca::concept&);
private:
  std::priority_queue<fca::concept>& coatoms;
  chain_map& chains;
};

class maxsub_visitor : public fca::arc_visitor {
public:
  maxsub_visitor(std::set<fca::concept>& mx) : maxsz(0), max(mx) {}
  void visitArc(const fca::concept&, const fca::concept&);
  const std::set<fca::concept>& maxSet() const { return max; }
private:
  std::size_t maxsz;
  std::set<fca::concept>& max;
};

void findAltChains(std::ostream&, std::priority_queue<fca::concept>&,
                  chain_map&, fca::lattice&);

};
#endif

#ifndef _lattice_h
#define _lattice_h

#include <map>
#include <iostream>
#include <string>
#include <queue>

#include "concept.hpp"

namespace fca {

class arc_visitor {
public:
  virtual void visitArc(const concept&, const concept&) = 0;
};

class lattice {
public:

  lattice() : lmap() {}
  lattice(const lattice& l) : lmap(l.lmap) {}

  void add(const concept& c1, const concept& c2);
  void accept(arc_visitor*) const;
  void accept(const concept&, arc_visitor*) const;

  typedef std::map<concept, std::set<concept> > lattice_map;

private:
  lattice_map lmap;
};

//only works if context is reduced --- no attributes associated with all objects
inline bool isTop(const concept& c) {
    return c.getAttributes().empty(); // || c.getObjects().size() = ALLOBJECTS ??
}

//only works if context is reduced --- no attributes associated with all objects
inline bool isBot(const concept& c) {
    return c.getObjects().empty(); // || c.getAttributes().size() == ALLATTRIBUTES
}

class uparc_visitor : public arc_visitor {
public:
  uparc_visitor(lattice& l): ltce(l) {};
  /* virtual */ void visitArc(const concept& super, const concept& sub) {
    ltce.add(sub,super); //up mapping
  }
private:
  lattice& ltce;
};

class downarc_visitor : public arc_visitor {
public:
  downarc_visitor(lattice& l): ltce(l) {};
  /* virtual */ void visitArc(const concept& super, const concept& sub) {
    ltce.add(super,sub); //down mapping
  }
private:
  lattice& ltce;
};

class arc_writer : public arc_visitor {
public:
  arc_writer(concept_writer* writer) : wrt(writer) {}
  /* virtual */ void visitArc(const concept&, const concept&);
private:
  concept_writer* wrt;
};

void readDotLattice(std::istream&, arc_visitor*);
void writeLattice(const lattice&, concept_writer*);
};
#endif

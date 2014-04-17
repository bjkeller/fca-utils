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
  void add(const concept& c1, const concept& c2);
  void accept(arc_visitor*) const;
  void accept(const concept&, arc_visitor*) const;
  typedef std::map<concept, std::set<concept> > lattice_map;
private:

  lattice_map lmap;
};

inline bool isTop(const concept& c) {
    return c.getAttributes().empty(); // || c.getObjects().size() = ALLOBJECTS ??
}

inline bool isBot(const concept& c) {
    return c.getObjects().empty(); // || c.getAttributes().size() == ALLATTRIBUTES
}




class uplattice_visitor : public arc_visitor {
public:
  uplattice_visitor(lattice& l): ltce(l) {};
  /* virtual */ void visitArc(const concept& super, const concept& sub) {
    ltce.add(sub,super); //up mapping
  }
private:
  lattice& ltce;
};

class downlattice_visitor : public arc_visitor {
public:
  downlattice_visitor(lattice& l): ltce(l) {};
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

void parseDotArc(const std::string&, arc_visitor*);
void readDotLattice(std::istream&, arc_visitor*);
void writeLattice(const lattice&, const concept_writer*);
};
#endif

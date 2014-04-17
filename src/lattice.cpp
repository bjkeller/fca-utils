#include "lattice.hpp"

#include <sstream>

namespace fca {

  using namespace std;

  void lattice::add(const concept& c1, const concept& c2) {
    set<concept> pset;
    lattice_map::iterator c_i = lmap.find(c1);
    if (c_i != lmap.end()) {
      c_i->second.insert(c2);
    } else {
      pset.insert(c2);
      lmap[c1] = pset;
    }
  }

  void lattice::accept(arc_visitor* v) const {
    lattice_map::const_reverse_iterator c_i = lmap.rbegin();
    while (c_i != lmap.rend()) {
      set<concept>::const_iterator s_i = c_i->second.begin();
      while (s_i != c_i->second.end()) {
        v->visitArc(c_i->first,*s_i);
        s_i++;
      }
      c_i++;
    }
  }

  void lattice::accept(const concept& c, arc_visitor* v) const {
    lattice_map::const_iterator c_i = lmap.find(c);
    if (c_i != lmap.end()) {
      set<concept>::const_iterator s_i = c_i->second.begin();
      while (s_i != c_i->second.end()) {
        v->visitArc(c_i->first,*s_i);
        s_i++;
      }
    }
  }


  void arc_writer::visitArc(const concept& super, const concept& sub) {
    wrt->write(super);
    (wrt->getStream()) << "->";
    wrt->write(sub);
    (wrt->getStream()) << endl;
  }


  set<string> parseList(const string& line) {
    string tok;
    set<string> result;
    istringstream ls(line);
    while (ls) {
      getline(ls,tok,',');
      tok.erase(remove_if(tok.begin(), tok.end(), ::isspace), tok.end());
      if (tok.length() > 0) {
        result.insert(tok);
      }
    }
    return result;
  }

  concept parseDotConcept(const string& line) {
    int spos = line.find(";");
    if (spos != string::npos) {
      set<string> objects;
      set<string> attributes;
      if (spos > 1) {
        objects= parseList(line.substr(1,spos-1)); //don't forget quotes
      }
      if (spos < line.length()-1) {
        attributes = parseList(line.substr(spos+1,line.length()-spos-2));
      }
      return concept(objects,attributes);
    }
    return concept();
  }

  // need for some polymorphism here!!!

  /*
  //may need to change for down up parsing
  //
  size_t parseArc(lattice_map& downMap, const string& line) {
  int apos = line.find("->");
  if (apos != string::npos) {
  concept super = parseConcept(line.substr(0,apos));
  concept sub = parseConcept(line.substr(apos+2)); // handle arc attributes here

  add(downMap,super,sub);
  return sub.getAttributes().size();
}
return 0;
}
*/





void parseDotArc(const std::string& line, arc_visitor* v) {
  int apos = line.find("->");
  if (apos != string::npos) {
    concept super = parseDotConcept(line.substr(0,apos));
    concept sub = parseDotConcept(line.substr(apos+2)); // handle arc attributes here

    v->visitArc(super,sub);
  }
}

void readDotLattice(istream& is, arc_visitor* v) {
  string line;
  getline(is,line);
  while (is) {
    parseDotArc(line,v);
    getline(is,line);
  }
}

void writeLattice(const lattice& l,  concept_writer* wrt) {
  cout << "digraph lattice { " << endl;

  arc_writer* v = new arc_writer(wrt);

  l.accept(v);

  delete v;

  cout << "}" << endl;
}

}; //namespace fca

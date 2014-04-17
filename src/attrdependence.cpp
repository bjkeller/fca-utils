//
//  attrdependence.cpp
//
//
//  Created by Benjamin Keller on 7/10/12.
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
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

using namespace std;

typedef map<concept, set<string> > attr_map;




void writeConcept(ostream& os, const concept& c, const attr_map& diffmap) {
  attr_map::const_iterator a_i = diffmap.find(c);
  if (a_i != diffmap.end()) {
    writeConceptDiff(os,c,a_i->second);

  } else {
    writeConcept(os,c);
  }

}

void writeDotArc(ostream& os, const concept& c1, const concept& c2, const attr_map& diffmap) {
  writeConcept(os,c1,diffmap);
  os << "->";
  writeConcept(os,c2,diffmap);
  if (c1.getAttributes().size() / static_cast<double>(c2.getAttributes().size()) < 0.5) {
    cout << " [color=\"crimson\" taillabel=\""
    << c1.getAttributes().size()
    << "\" headlabel=\""
    << c2.getAttributes().size() << "\"]";
  }
  os << endl;

}

void writeLattice(ostream& os, const lattice_map& downMap, const attr_map& diffmap) {

  set<string> diffset;

  os << "digraph lattice {" << endl;

  lattice_map::const_reverse_iterator c_i = downMap.rbegin();
  while (c_i != downMap.rend()) {

    set<concept>::const_iterator s_i = c_i->second.begin();
    while (s_i != c_i->second.end()) {
      writeDotArc(os,c_i->first,*s_i,diffmap);
      s_i++;
    }
    c_i++;
  }

  os << "}" << endl;
}

void filterLattice(const lattice_map& upMap, lattice_map& downMap, attr_map& diffmap) {

  set<concept> visited;
  queue<concept> tovisit;

  lattice_map::const_iterator u_i = upMap.begin();
  if (u_i != upMap.end()) { //bottom -- less than everything
    // set<string> atomattr;
    set<concept>::const_iterator s_i;
    s_i = u_i->second.begin();
    while (s_i != u_i->second.end()) {
      add(downMap,*s_i,u_i->first);
      //atomattr.insert((*s_i).getAttributes().begin(), (*s_i).getAttributes().end());
      tovisit.push(*s_i);
      s_i++;
    }
    //diffmap[u_i->first] = atomattr;
  }

  while (!tovisit.empty()) {
    concept next = tovisit.front();
    tovisit.pop();
    if (visited.find(next) == visited.end()) { //not visited
      visited.insert(next); //mark visited
      u_i = upMap.find(next);
      if (u_i != upMap.end()) {
        size_t maxsz = 0;
        set<concept> max;
        // set<string> attr;
        set<concept>::const_iterator s_i = u_i->second.begin();
        while (s_i != u_i->second.end()) {
          size_t sz = (*s_i).getAttributes().size();

          if (sz >= maxsz) {
            if (sz > maxsz) {
              max = set<concept>();
              maxsz = sz;
            }
            max.insert(*s_i);


          }
          // attr.insert((*s_i).getAttributes().begin(),(*s_i).getAttributes().end());
          s_i++;
        }


        s_i = max.begin();
        while (s_i != max.end()) {
          add(downMap,*s_i,u_i->first);
          tovisit.push(*s_i);
          s_i++;
        }
        // diffmap[u_i->first] = attr;

      } else {
        cerr << "badness: didn't find" << endl;
        writeConcept(cerr,next);
        cerr << "in upMap" << endl;
      }
    }
  }


}

int main(int argc, char* argv[]) {
  lattice_map upMap;
  readlattice(cin,upMap);

  lattice_map downMap;
  attr_map diffmap;

  filterLattice(upMap,downMap,diffmap);

  writeLattice(cout,downMap,diffmap);

}

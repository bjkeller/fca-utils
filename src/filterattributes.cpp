//
//  filterattributes.cpp
//
//
//  Created by Benjamin Keller on 5/9/12.
//  Copyright (c) 2012 Benjamin J. Keller. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <algorithm>



#include "concept.h"

using namespace std;

typedef map<concept, set<string> > attr_map;

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
        if (visited.find(next) == visited.end()) {
            visited.insert(next);
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

void filterAttributes(const lattice_map& upMap, lattice_map& downMap, attr_map& diffmap) {

    set<concept> visited;
    queue<concept> tovisit;

    lattice_map::const_iterator u_i = upMap.begin();
    if (u_i != upMap.end()) { //bottom -- less than everything
        set<string> atomattr;
        set<concept>::const_iterator s_i;
        s_i = u_i->second.begin();
        while (s_i != u_i->second.end()) {
            add(downMap,*s_i,u_i->first);
            atomattr.insert((*s_i).getAttributes().begin(), (*s_i).getAttributes().end());
            tovisit.push(*s_i);
            s_i++;
        }
        diffmap[u_i->first] = atomattr;
    }

    while (!tovisit.empty()) {
        concept next = tovisit.front();
        tovisit.pop();
        if (visited.find(next) == visited.end()) {
            visited.insert(next);
            u_i = upMap.find(next);
            if (u_i != upMap.end()) {


                set<string> attr;
                set<concept>::const_iterator s_i = u_i->second.begin();
                while (s_i != u_i->second.end()) {
                    add(downMap,*s_i,u_i->first);
                    tovisit.push(*s_i);
                    attr.insert((*s_i).getAttributes().begin(),(*s_i).getAttributes().end());

                    s_i++;
                }


                diffmap[u_i->first] = attr;

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

    filterAttributes(upMap,downMap,diffmap);

    writeLattice(cout,downMap,diffmap);

}

//
//  tablefilter.cpp
//
//  Reads dot file output of colibri-java and creates table with concept entropies 
//
//  Created by Benjamin Keller on 10/24/2013
//  Copyright (c) 2013 Benjamin James Keller. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>


#include "concept.h"

using namespace std;

set<string> parseList(const string& line) {
    string tok;
    set<string> result;
    istringstream ls(line);
    while (ls) {
        getline(ls,tok,',');
        tok.erase(remove_if(tok.begin(), tok.end(), ::isspace), tok.end());
        result.insert(tok);
    }
    return result;
}

concept parseConcept(const string& line) {
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

size_t parseArc(set<concept>& cset, const string& line) {
    int apos = line.find("->");
    if (apos != string::npos) {
        concept super = parseConcept(line.substr(0,apos));
        concept sub = parseConcept(line.substr(apos+2)); // handle arc attributes here

        cset.insert(super);
        cset.insert(sub);
        return sub.getAttributes().size();
    }
    return 0;
}

void writeConcept(ostream& os, const concept& c, size_t attrcnt) {
  set<string> s = c.getObjects();
  set<string>::const_iterator s_i = s.begin();
  os << "\"";
  while (s_i != s.end()) {
    os << *s_i++;
    if (s_i != s.end()) {
      os << ",";
    }
  }
  os << "\",";
  double entropy = 0.0;
  if (!c.getAttributes().empty() && c.getAttributes().size() != attrcnt) {
    double p_con = c.getAttributes().size() / static_cast<double>(attrcnt);
    double log_p_con = log2(p_con);
    entropy = -1*(p_con*log_p_con);
  }
  os << c.getAttributes().size() << ",";
  os << entropy;
  os << endl;
}

int main(int argc, char* argv[]) {
  set<concept> cset;
  size_t attrcnt = 0;

  string line;
  getline(cin,line);
  while (cin) {
    size_t cnt = parseArc(cset,line);
    if (cnt > attrcnt) {
      attrcnt = cnt;
    }
    getline(cin,line);
  }

  set<concept>::iterator c_i = cset.begin();
  while (c_i != cset.end()) {
    writeConcept(cout,*c_i,attrcnt);
    c_i++;
  }

}

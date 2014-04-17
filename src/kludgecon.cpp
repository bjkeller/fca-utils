//
//  kludgecon.cpp
//
// fixes con file so that any whitespace in object names is replaced
// really should just fix Colibri-Java FCA CON read, but this is the kludge
//
//  Created by Benjamin Keller on 12/4/12.
//  Copyright (c) 2012 Benjamin J. Keller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>


using namespace std;

typedef map<string, set<string> > con_map;

void readLine(const string& line, con_map& cmap) {
    istringstream ls(line);
    if (ls) {
        string object;
        getline(ls,object,':');
        replace(object.begin(),object.end(),' ','-');
        set<string> attrs;
        string attribute;
        while (ls >> attribute) {
            int spos = attribute.find(";");
            if (spos != string::npos) {
                attribute.erase(spos,1);
            }
            attrs.insert(attribute);
        }
        cmap[object] = attrs;
    }
}

void readCon(istream& is, con_map& cmap) {
    if (is) {
        string line;
        getline(is,line);
        while (is) {
            readLine(line,cmap);
            getline(is,line);
        }
    }
}

void writeCon(ostream& os, const con_map& cmap) {
    con_map::const_iterator c_i = cmap.begin();
	while (c_i != cmap.end()) {
		os << c_i->first << ":";
		set<string>::const_iterator s_i = c_i->second.begin();
		while (s_i != c_i->second.end()) {
			os << " " << *s_i;
			s_i++;
		}
		os << ";" << endl;
		c_i++;
	}
}

int main(int argc, char* argv[]) {

    con_map cmap;
    readCon(cin,cmap);

    writeCon(cout,cmap);

}

// table2concept.cpp
// reads object attribute pairs and convers to con format for colibri-java
//
//  Copyright (c) 2012 Benjamin James Keller. All rights reserved.

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <set>

#include "context.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	fca::Context attrmap;
	string line;
	getline(cin,line);
	while(cin) {
		istringstream ls(line);
		string obj, attr;
		if (ls >> obj >> attr) {
			attrmap.add(obj,attr);
		}
		getline(cin,line);
	}

	writeCon(cout,attrmap);

}

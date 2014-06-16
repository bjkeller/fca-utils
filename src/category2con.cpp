/*
 * Copyright (c) 2014 Benjamin J. Keller. All rights reserved.
 */

#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "context.hpp"

using namespace std;

string makeAttr(const string& attrname, size_t i, const string& val) {
  ostringstream oss;
  oss << attrname << i << "_" << val;
  return oss.str();
}

void readTableLine(const string& line,
                   const string& obj, const string& attrname,
                   fca::Context& ctxt) {
  istringstream ls(line);
  if (ls) {
    size_t col = 0;
    string tok;
    while(ls >> tok) {
      string attr = makeAttr(attrname,col,tok);
      col++;
      ctxt.add(obj,attr);
    }
  }
}

string makeObj(const string& objname, size_t i) {
  ostringstream oss;
  oss << objname << i;
  return oss.str();
}

void readTable(istream& is, fca::Context& cmap) {
  string objname = "s";
  string attrname = "a";
  if (is) {
    size_t row = 0;
    string line;
    getline(is,line);
    while(is) {
      readTableLine(line,makeObj(objname,row),attrname,cmap);
      getline(is,line); row++;
    }
  }
}

int main(int argc, char* argv[]) {
  fca::Context ctxt;
  readTable(std::cin,ctxt);
  fca::writeCon(std::cout,ctxt);
}

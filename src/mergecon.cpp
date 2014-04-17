//
//  mergecon.cpp
//
//
//  Created by Benjamin Keller on 6/6/12.
//  Copyright (c) 2012 Benjamin J. Keller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>

#include "context.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  string f1name = argv[1];
  string f2name = argv[2];
  string ofname = argv[3];

  fca::Context cmap;
  ifstream is1(f1name.c_str());
  readCon(is1,cmap);

  fca::Context cmap2;
  ifstream is2(f2name.c_str());
  readCon(is2,cmap2);

  cmap.merge(cmap2);

  ofstream os(ofname.c_str());
  writeCon(os,cmap);
  os.close();
}

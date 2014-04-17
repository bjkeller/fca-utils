//
//  restrictcon.cpp
//
//  Modifies incidence relation of objects with attributes by eliminating
//  either objects (whole rows) or attributes ("right" restriction).
//  Reads/writes "con" file format read by colibri-java FCA code.
//
//  Created by Benjamin Keller on 6/5/12.
//  Copyright (c) 2012 Benjamin James Keller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <algorithm>

#include "tclap/CmdLine.h"

#include "context.hpp"

using namespace std;
using namespace TCLAP;

void readList(const string& file, set<string>& oset) {
  fstream is(file.c_str());
  if (is) {
    string line;
    getline(is,line);
    while (is) {
      oset.insert(line);
      getline(is,line);
    }
  }
  is.close();
}

bool parse_arguments(int argc, char* argv[], string& confilename, string& setfilename, string& outfilename, bool& right_restrict) {
  try {
    CmdLine cmd("restrictcon", ' ', "0.0000549er");

    ValueArg<string> cfile("c","confile","context file",true,"ConFilename","string");
    ValueArg<string> lfile("l","listfile","restriction list",true,"ListFilename","string");
    ValueArg<string> ofile("o","outfile","output context file",true,"OutFilename","string");
    SwitchArg right_switch("r","right","right restriction",false);

    cmd.add(cfile);
    cmd.add(lfile);
    cmd.add(ofile);
    cmd.add(right_switch);
    cmd.parse(argc,argv);

    confilename = cfile.getValue();
    setfilename = lfile.getValue();
    outfilename = ofile.getValue();
    right_restrict = right_switch.isSet();

    return true;
  }
  catch (ArgException& e) {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return false;
  }
}

int main(int argc, char* argv[]) {

  string confilename;
  string setfilename;
  string outfilename;
  bool right_restrict = false;

  bool valid_arguments = parse_arguments(argc,argv,confilename,setfilename,outfilename,right_restrict);

  if (valid_arguments) {

    fca::Context cmap_in;
    ifstream is1(confilename);
    readCon(is1,cmap_in);

    set<string> restriction_set;
    readList(setfilename,restriction_set);

    fca::Context cmap;
    if (right_restrict) {
      cmap.right_restrict(restriction_set,cmap_in);
    } else {
      cmap.left_restrict(restriction_set,cmap_in);
    }

    ofstream os(outfilename.c_str());
    writeCon(os,cmap);
    os.close();

  }
}

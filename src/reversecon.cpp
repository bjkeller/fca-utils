//
//  reversecon.cpp
//
//  Flips objects and attributes of incidence relation
//  Reads/writes "con" file format read by colibri-java FCA code.
//
//  Created by Benjamin Keller on 9/11/2013
//  Copyright (c) 2013 Benjamin James Keller. All rights reserved.
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

bool parse_arguments(int argc, char* argv[], string& confilename, string& outfilename) {
  try {
    CmdLine cmd("reversecon", ' ', "0.0000549er");

    ValueArg<string> cfile("c","confile","context file",true,"ConFilename","string");
    ValueArg<string> ofile("o","outfile","output context file",true,"OutFilename","string");

    cmd.add(cfile);
    cmd.add(ofile);
    cmd.parse(argc,argv);

    confilename = cfile.getValue();
    outfilename = ofile.getValue();

    return true;
  }
  catch (ArgException& e) {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
    return false;
  }
}

int main(int argc, char* argv[]) {

  string confilename;
  string outfilename;

  bool valid_arguments = parse_arguments(argc,argv,confilename,outfilename);

  if (valid_arguments) {

    fca::Context cmap_in;
    ifstream is1(confilename);
    readCon(is1,cmap_in);

    fca::Context cmap;
    cmap.transpose(cmap_in);

    ofstream os(outfilename.c_str());
    writeCon(os,cmap);
    os.close();

  }
}

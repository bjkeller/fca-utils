//
//  composecon.cpp
//
//
//  Created by Benjamin Keller on 5/16/12.
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <string>

#include "context.hpp"

int main(int argc, char* argv[]) {
    std::string f1name = argv[1];
    std::string f2name = argv[2];
    std::string ofname = argv[3];

    fca::Context c1;
    std::ifstream is1(f1name.c_str());
    fca::readCon(is1,c1);
    is1.close();

    fca::Context c2;
    std::ifstream is2(f2name.c_str());
    fca::readCon(is2,c2);
    is2.close();

    fca::Context ctxt;
    ctxt.compose(c1,c2);

    std::ofstream os(ofname.c_str());
    fca::writeCon(os,ctxt);
    os.close();

}

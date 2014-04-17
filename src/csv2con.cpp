//  Copyright (c) 2012 Benjamin Keller. All rights reserved.

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <set>

#include "context.hpp"

using namespace std;

void readCSVLine(const string& line, fca::Context& cmap) {
    if (line[0] != '#') {
        istringstream ls(line);
        if (ls) {
            string object;
            string attribute;
            set<string> attrs;
            string tok;
            if (ls >> tok) {
                ostringstream oss;
                if (tok[0] != '\"') {
                    if (tok.find(',') == string::npos) {
                        oss << tok;
                        while (ls >> tok && tok.find(',') == string::npos) {
                            oss << '-' << tok;
                        }
                        oss << '-' << tok.substr(0,tok.find(','));
                    } else {
                        oss << tok.substr(0,tok.find(','));
                    }
                    tok = tok.substr(tok.find(',')+1);
                } else { //quoted
                    if (tok.find(',') == string::npos) {
                        oss << tok.substr(1);
                    } else {
                        oss << tok.substr(1,tok.find(',')-1);
                    }

                    while (ls >> tok && tok.find('\"') == string::npos) {
                        oss << '-';
                        if (tok.find(',' != string::npos)) {
                            oss << tok.substr(0,tok.find(','));
                        } else {
                            oss << tok;
                        }
                    }
                    oss << '-' << tok.substr(0,tok.find('\"'));
                    tok = tok.substr(tok.find(',')+1);
                }
                object = oss.str();

                if (tok[0] != '\"') {
                    attrs.insert(tok);
                    while (ls >> tok) {
                        attrs.insert(tok);
                    }
                } else {
                    attrs.insert(tok.substr(1,tok.find(',')-1));
                    while (ls >> tok && tok.find('\"') == string::npos) {
                        attrs.insert(tok.substr(0,tok.find(',')));
                    }
                    attrs.insert(tok.substr(0,tok.find('\"')));
                }
            }



            cmap.add(object,attrs);
        }
    }
}

void readCSV(istream& is, fca::Context& cmap) {
    if (is) {
        string line;
        getline(is,line);
        while (is) {
            readCSVLine(line,cmap);
            getline(is,line);
        }
    }
}

int main(int argc, char* argv[]) {
    fca::Context ctxt;
    readCSV(cin,ctxt);
    writeCon(cout,ctxt);
}

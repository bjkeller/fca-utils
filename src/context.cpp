//
//  context.cpp
//
//
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
//

#include "context.hpp"

#include <sstream>

namespace fca {

using namespace std;

void Context::add(const string& object, const string& attribute) {
  set<string> attrs;
  map<string, set<string> >::iterator o_i = cmap.find(object);
  if (o_i != cmap.end()) {
    attrs = o_i->second;
  }
  attrs.insert(attribute);
  cmap[object] = attrs;
}

void Context::add(const string& object, const set<string>& attrs) {
  set<string> attributes;
  map<string, set<string> >::iterator o_i = cmap.find(object);
  if (o_i != cmap.end()) {
    attributes = o_i->second;
  }
  attributes.insert(attrs.begin(),attrs.end());
  cmap[object] = attributes;
}

void readConLine(const string& line, Context& ctxt) {
  istringstream ls(line);
  if (ls) {
    string object;
    getline(ls,object,':');
    set<string> attrs;
    string attribute;
    while (ls >> attribute) {
      int spos = attribute.find(";");
      if (spos != string::npos) {
        attribute.erase(spos,1);
      }
      attrs.insert(attribute);
    }
    ctxt.add(object,attrs);
  }
}

void readCon(istream& is, Context& ctxt) {
  if (is) {
    string line;
    getline(is,line);
    while (is) {
      readConLine(line,ctxt);
      getline(is,line);
    }
  }
}

void writeCon(ostream& os, const Context& ctxt) {
  map<string, set<string> >::const_iterator c_i = ctxt.cmap.begin();
  while (c_i != ctxt.cmap.end()) {
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

void Context::merge(const Context& ctxt) {
  map<string, set<string> >::const_iterator c_i = ctxt.cmap.begin();
  while (c_i != ctxt.cmap.end()) {
    set<string> attrs;

    map<string, set<string> >::iterator r_i = cmap.find(c_i->first);
    if (r_i != cmap.end()) {
      attrs = r_i->second;
    }
    attrs.insert(c_i->second.begin(),c_i->second.end());

    cmap[c_i->first] = attrs;
    c_i++;
  }
}


void Context::left_restrict(const set<string>& oset, const Context& ctxt) {
  cmap.clear();
  set<string>::const_iterator o_i = oset.begin();
  while (o_i != oset.end()) {
    set<string> attrs;
    map<string, set<string> >::const_iterator c_i = ctxt.cmap.find(*o_i);
    if (c_i != ctxt.cmap.end()) {
      attrs.insert(c_i->second.begin(),c_i->second.end());
    }
    cmap[*o_i] = attrs;
    o_i++;
  }
}

void Context::right_restrict(const set<string>& oset, const Context& ctxt) {
  cmap.clear();
  map<string, set<string> >::const_iterator c_i = ctxt.cmap.begin();
  while (c_i != ctxt.cmap.end()) {
    set<string> attrs;

    set_intersection(c_i->second.begin(),c_i->second.end(),oset.begin(),oset.end(),insert_iterator<set<string> >(attrs,attrs.begin()));

    if (attrs.size() > 0) {
      cmap[c_i->first] = attrs;
    }

    c_i++;
  }
}

void Context::transpose(const Context& ctxt) {
  cmap.clear();
  map<string, set<string> >::const_iterator c_i = ctxt.cmap.begin();
  while (c_i != ctxt.cmap.end()) {

    set<string>::const_iterator a_i = c_i->second.begin();
    while (a_i != c_i->second.end()) {

      add(*a_i,c_i->first);

      a_i++;
    }

    c_i++;
  }
}

void Context::compose(const Context& ctxt1, const Context& ctxt2) {
  cmap.clear();
  map<string, set<string> >::const_iterator c_i = ctxt1.cmap.begin();
  while (c_i != ctxt1.cmap.end()) {
    set<string> attrs;
    set<string>::const_iterator s_i = c_i->second.begin();
    while (s_i != c_i->second.end()) {
      map<string, set<string> >::const_iterator c2_i = ctxt2.cmap.find(*s_i);
      if (c2_i != ctxt2.cmap.end()) {
        attrs.insert(c2_i->second.begin(),c2_i->second.end());
      }
      s_i++;
    }
    cmap[c_i->first] = attrs;
    c_i++;
  }
}

}; //namespace fca

//
//  context.h
//
//
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
//

#ifndef _confile_h
#define _confile_h

#include <iostream>
#include <map>
#include <set>
#include <string>

namespace fca {

class Context {
public:
  Context() : cmap() {}
  Context(const Context& ctxt): cmap(ctxt.cmap) {}

  void add(const std::string&,const std::string&);
  void add(const std::string&,const std::set<std::string>&);

  void merge(const Context& ctxt);
  void left_restrict(const std::set<std::string>& oset, const Context& ctxt);
  void right_restrict(const std::set<std::string>& oset, const Context& ctxt);
  void transpose(const Context& ctxt);
  void compose(const Context& ctxt1, const Context& ctxt2);

private:
  std::map<std::string, std::set<std::string> > cmap;
  friend void writeCon(std::ostream&, const Context&);
};

void writeCon(std::ostream& os, const Context& ctxt);
void readCon(std::istream& is, Context& ctxt);

};//end namespace fca
#endif

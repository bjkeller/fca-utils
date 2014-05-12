#ifndef _chain_h
#define _chain_h
#include <vector>
#include <cstddef>

#include <boost/shared_ptr.hpp>

#include "concept.hpp"
namespace fca {

class complexchain;
typedef boost::shared_ptr<complexchain> chain_ptr;

class chain_visitor;
typedef boost::shared_ptr<chain_visitor> chain_visitor_ptr;

//want that pushing makes a new chain that refs old chain in such a way that don't
//duplicate, but can still get front so that can write
class complexchain {
public:
  complexchain() : c(), con() {}
  complexchain(const chain_ptr& p, const concept& ccpt) : c(p), con(ccpt) {}

  void accept(chain_visitor_ptr) const;

  std::size_t length() const;
private:
  std::size_t count(const chain_ptr&,std::size_t) const;
  chain_ptr c;
  concept con;
};



class chain {
public:
  chain() : c(chain_ptr()) {}
  chain(const chain& ch) : c(ch.c) {}

  size_t length() const;
  void accept(chain_visitor_ptr v) const;

  chain push(const concept&);

private:
  chain(chain_ptr& p) : c(p) {}
  chain_ptr c;
};

//PONDER: maybe just replace these with arc_visitors?
class chain_visitor {
public:
  virtual void visit(const concept&) = 0;
};

class dot_chain_visitor : public chain_visitor {
public:
  dot_chain_visitor(std::ostream& out) : cwrt(new dot_writer(out)), prev(), first(true) {}
  /*virtual*/ void visit(const concept&);
private:
  concept_writer_ptr cwrt;
  concept prev;
  bool first;
};


class csv_chain_visitor : public chain_visitor {
public:
  csv_chain_visitor(concept_writer_ptr v) : cwrt(v), first(true) {}
  /*virtual*/ void visit(const concept&);
private:
  concept_writer_ptr cwrt;
  bool first;
};

};//end namespace fca

#endif

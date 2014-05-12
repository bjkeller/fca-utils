#include "chain.hpp"

namespace fca {

  size_t chain::length() const {
    if (!c) {
      return 0;
    } else {
      return c->length();
    }
  }

  chain chain::push(const concept& con) {
    chain_ptr p(new complexchain(c,con));
    return chain(p);
  }

  void chain::accept(chain_visitor_ptr v) const { c->accept(v); }

  void complexchain::accept(chain_visitor_ptr v) const {
    if (c) {
      c->accept(v);
    }
    v->visit(con);
  }

  std::size_t complexchain::count(const chain_ptr& p,std::size_t cnt) const {
    if (!p) {
      return cnt;
    } else {
      return count(p->c,cnt+1);
    }
  }

  std::size_t complexchain::length() const {
    return count(c,1);
  }


  //NOTE: this is almost identical to arc_writer::visitArc
  //
  void dot_chain_visitor::visit(const concept& con) {
    if (!first) {
      cwrt->write(prev);
      (cwrt->getStream()) << "->";
      cwrt->write(con);
      (cwrt->getStream())<< "\n";
    } else {
      first = false;
    }
    prev = con;
  }

  void csv_chain_visitor::visit(const concept& con) {
    if (!first) {
      (cwrt->getStream()) << ",";
    } else {
      first = false;
    }
    cwrt->write(con);
  }

};

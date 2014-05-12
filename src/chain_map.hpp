#ifndef _chain_map_h
#define _chain_map_h 1

#include <map>
#include <string>

#include "concept.hpp"
#include "chain.hpp"

namespace fca {

  class chain_map {
  public:
    chain_map() : cmap() {}
    chain_map(const chain_map& c) : cmap(c.cmap) {}

    void add(const concept&);
    bool push(const concept&, const std::set<concept>&);

    size_t size() const { return cmap.size(); }
  private:
    std::map<concept,chain> cmap;
  };

};

#endif

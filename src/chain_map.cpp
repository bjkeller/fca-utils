#include "chain_map.hpp"

#include <iostream>

namespace fca {

bool chain_map::push(const concept& tail, const std::set<concept>& cset) {

  std::map<concept,chain>::iterator ch_i = cmap.find(tail);
  if (ch_i != cmap.end()) {
    chain tail_chain = ch_i->second;
    cmap.erase(ch_i);

    std::set<concept>::const_iterator s_i = cset.begin();
    while (s_i != cset.end()) {
      cmap[*s_i] = tail_chain.push(*s_i);
      s_i++;
    }
    return true;
  } else {
    return false;
  }
}

void chain_map::add(const concept& con) {

  if (cmap.find(con) == cmap.end()) {
    chain ch;
    cmap[con] = ch.push(con);
  }

}

};

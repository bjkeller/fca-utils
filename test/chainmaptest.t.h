#include <cxxtest/TestSuite.h>

#include "chain_map.hpp"

class ChainMapTest: public CxxTest::TestSuite {
public:
  void testPush(void) {
    std::set<std::string> oset1, oset2, oset3, oset4, aset;
    oset1.insert("o1");
    oset2.insert("o2");
    oset3.insert("o3");
    oset4.insert("o4");

    fca::chain_map mp1;
    fca::concept c1(oset1,aset),c2(oset2,aset), c3(oset3,aset), c4(oset4,aset);
    std::set<fca::concept> cset;
    cset.insert(c2);

//have to add chain before pushing to it
    TS_ASSERT(!mp1.push(c1,cset));
    TS_ASSERT_EQUALS(mp1.size(),0);

//adding to chain doesn't change it's size
    mp1.add(c1);
    TS_ASSERT_EQUALS(mp1.size(),1);
    TS_ASSERT(mp1.push(c1,cset));
    TS_ASSERT_EQUALS(mp1.size(),1);

    std::set<fca::concept> cset2;
    cset2.insert(c3);
    cset2.insert(c4);

//adding to earlier concept in chain doesn't work
    TS_ASSERT(!mp1.push(c1,cset2));
    TS_ASSERT_EQUALS(mp1.size(),1);

    TS_ASSERT(mp1.push(c2,cset2));
    TS_ASSERT_EQUALS(mp1.size(),2);

  }
};

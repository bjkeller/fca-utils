#include <cxxtest/TestSuite.h>

#include "chain.hpp"

class ChainTest: public CxxTest::TestSuite {
public:

  void testChainLength(void){
    fca::chain ch, ch1;
    //a new chain has length zero
    TS_ASSERT_EQUALS(ch.length(),0);

    fca::concept c1, c2, c3;
    ch1 = ch.push(c1);
    TS_ASSERT_EQUALS(ch.length(),0);
    TS_ASSERT_EQUALS(ch1.length(),1);

    ch1 = ch1.push(c2);
    TS_ASSERT_EQUALS(ch1.length(),2);
  }

  void testChainVisit(void) {
    std::set<std::string> oset1, oset2, oset3;
    std::set<std::string> aset1, aset2, aset3;

    oset1.insert("o1"); oset2.insert("o1"); oset3.insert("o1");
    oset2.insert("o2"); oset3.insert("o2");
    oset3.insert("o3");

    aset3.insert("a3"); aset2.insert("a3"); aset1.insert("a3");
    aset2.insert("a2"); aset1.insert("a2");
    aset1.insert("a1");

    fca::concept c1(oset1,aset1), c2(oset2,aset2), c3(oset3,aset3);

    fca::chain ch;
    ch = ch.push(c1);
    ch = ch.push(c2);
    ch = ch.push(c3);

    std::ostringstream os;
    fca::chain_visitor_ptr dcv(new fca::dot_chain_visitor(os));
    ch.accept(dcv);

    std::string expected = "\"o1;a1, a2, a3\"->\"o1, o2;a2, a3\"\n\"o1, o2;a2, a3\"->\"o1, o2, o3;a3\"\n";
    TS_ASSERT_EQUALS(os.str(),expected);

    std::ostringstream os2;
    fca::concept_writer_ptr ccw(new fca::csv_writer(os2,3));
    fca::chain_visitor_ptr ccv(new fca::csv_chain_visitor(ccw));
    ch.accept(ccv);
    std::string csv_expected = "\"o1\",\"a1, a2, a3\",3,0,\"o1, o2\",\"a2, a3\",2,0.389975,\"o1, o2, o3\",\"a3\",1,0.528321";
    TS_ASSERT_EQUALS(os2.str(),csv_expected);
  }



};

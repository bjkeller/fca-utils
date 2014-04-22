#include <cxxtest/TestSuite.h>

#include <sstream>

#include "context.hpp"

class ContextTests : public CxxTest::TestSuite {

public:

  void testEmpty(void) {
    fca::Context con;
    std::ostringstream os;
    writeCon(os,con);

    TS_ASSERT_EQUALS(os.str().length(),0);
  }

  void testSingleon(void) {
    fca::Context con;
    std::ostringstream os;

    con.add("a","b");
    writeCon(os,con);

    TS_ASSERT_EQUALS(os.str(),"a: b;\n");
  }

  void testPair(void) {
    fca::Context con;
    std::ostringstream os;

    con.add("a","b1");
    con.add("a","b2");
    con.add("c","b3");
    writeCon(os,con);

    std::string expected = "a: b1 b2;\nc: b3;\n";
    TS_ASSERT_EQUALS(os.str(),expected);
  }

  void testSet(void) {
    fca::Context emptycon, con;
    std::ostringstream emptyos,os;
    std::set<std::string> empty, s1;

    emptycon.add("a",empty);
    writeCon(emptyos,emptycon);
    TS_ASSERT_EQUALS(os.str().length(),0);

    s1.insert("b1");
    s1.insert("b2");
    s1.insert("b3");
    con.add("a",s1);
    writeCon(os,con);

    std::string expected = "a: b1 b2 b3;\n";
    TS_ASSERT_EQUALS(os.str(),expected);
  }

  void testMerge(void) {
    fca::Context con1, con2, con3;
    std::ostringstream mtos1, mtos2;
    con1.merge(con2);  //empty with empty
    writeCon(mtos1,con1);
    TS_ASSERT_EQUALS(mtos1.str().length(),0);
    writeCon(mtos2,con2);
    TS_ASSERT_EQUALS(mtos2.str().length(),0);

    std::ostringstream os1,os2;
    con1.add("a","b1");
    con1.merge(con2); //not empty with empty
    writeCon(os1,con1);
    writeCon(os2,con2);
    TS_ASSERT_EQUALS(os1.str(),"a: b1;\n");
    TS_ASSERT_EQUALS(os2.str(),"");

    std::ostringstream os11,os12;
    con3.merge(con1); //empty with not empty
    writeCon(os11,con1);
    writeCon(os12,con3);
    TS_ASSERT_EQUALS(os11.str(),"a: b1;\n");
    TS_ASSERT_EQUALS(os12.str(),"a: b1;\n");

    std::ostringstream os21, os22;
    con1.merge(con3); //equal
    writeCon(os21,con1);
    writeCon(os22,con3);
    TS_ASSERT_EQUALS(os11.str(),"a: b1;\n");
    TS_ASSERT_EQUALS(os12.str(),"a: b1;\n");

    con2.add("a1","b2");
    std::ostringstream os31, os32;
    con1.merge(con2);
    writeCon(os31,con1);
    writeCon(os32,con2);
    TS_ASSERT_EQUALS(os31.str(),"a: b1;\na1: b2;\n");
    TS_ASSERT_EQUALS(os32.str(),"a1: b2;\n");
  }

  void testRestrict(void) {
    fca::Context con1;
    con1.add("a1","b1");
    con1.add("a2","b2");

    std::ostringstream os1;
    writeCon(os1,con1);
    std::string constr1 = os1.str();

    std::set<std::string> oset; //empty
    fca::Context con2;
    con2.left_restrict(oset,con1);
    std::ostringstream os2;
    writeCon(os2,con2);
    TS_ASSERT_EQUALS(os2.str(),"");

    con2.right_restrict(oset,con1);
    std::ostringstream os3;
    writeCon(os3,con2);
    TS_ASSERT_EQUALS(os3.str(),"");

    oset.insert("a1");
    con2.left_restrict(oset,con1);
    std::ostringstream os4;
    writeCon(os4,con2);
    TS_ASSERT_EQUALS(os4.str(),"a1: b1;\n");

    fca::Context con3;
    con3.right_restrict(oset,con1);
    std::ostringstream os5;
    writeCon(os5,con3);
    TS_ASSERT_EQUALS(os5.str(),"");

    std::set<std::string> rset;
    rset.insert("b2");
    con3.right_restrict(rset,con1);
    std::ostringstream os6;
    writeCon(os6,con3);
    TS_ASSERT_EQUALS(os6.str(),"a2: b2;\n");

    //overwrites!
    con3.left_restrict(oset,con1);
    std::ostringstream os7;
    writeCon(os7,con3);
    TS_ASSERT_EQUALS(os7.str(),"a1: b1;\n");

    //make sure cons1 didn't change
    std::ostringstream os8;
    writeCon(os8,con1);
    TS_ASSERT_EQUALS(os8.str(),constr1);
  }

  void testTranspose(void) {
    fca::Context con1, con2;

    con1.transpose(con2);
    std::ostringstream os1;
    writeCon(os1,con1);
    TSM_ASSERT_EQUALS("transpose of empty context is empty",os1.str(),"");

    con1.add("a1","b1");
    con1.add("a2","b2");
    con2.transpose(con1);
    std::ostringstream os2;
    writeCon(os2,con2);
    TS_ASSERT_EQUALS(os2.str(),"b1: a1;\nb2: a2;\n");

  }

  void testCompose(void) {
    fca::Context con1, con2, con3;

    con1.compose(con2,con3);
    std::ostringstream os1;
    writeCon(os1,con1);
    TS_ASSERT_EQUALS(os1.str(),"");

    con2.add("a1","b1");
    con1.compose(con2,con3);
    writeCon(os1,con1);
    TS_ASSERT_EQUALS(os1.str(),"");

    con1.compose(con3,con2);
    writeCon(os1,con1);
    TS_ASSERT_EQUALS(os1.str(),"");

    con3.add("b2","c2");
    con1.compose(con2,con3);
    writeCon(os1,con1);
    TS_ASSERT_EQUALS(os1.str(),"");

    con3.add("b1","c1");
    con1.compose(con2,con3);
    writeCon(os1,con1);
    TS_ASSERT_EQUALS(os1.str(),"a1: c1;\n");
  }

  void testRead(void) {
    fca::Context con;

    std::string line = "a1: b1;\na2: b1 b2;\n";
    std::istringstream is(line);
    readCon(is,con);
    std::ostringstream os;
    writeCon(os,con);

    TS_ASSERT_EQUALS(os.str(),line);
  }
};

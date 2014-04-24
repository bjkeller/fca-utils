#include <cxxtest/TestSuite.h>

#include "concept.hpp"

//
// remember that a "concept" is really just a pair of sets, so really not a
// formal concept at all
//
class ConceptTests: public CxxTest::TestSuite {
public:

  void testEmpty(void) {
    fca::concept con1,con2;
    TS_ASSERT_EQUALS(con1,con2);
    TS_ASSERT_EQUALS(con1.getAttributes().size(),0);
    TS_ASSERT_EQUALS(con1.getObjects().size(),0);
    TS_ASSERT(!(con1 < con2));
    TS_ASSERT_EQUALS(con1.entropy(100),0);
  }

  void testComparison(void) {
    std::set<std::string> objs0, objs1, objs2, objs3, objs4;
    std::set<std::string> attrs0, attrs1, attrs2, attrs3, attrs4;

    //want con0 < con1 < con2 < con3
    attrs0.insert("a1");
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs4.insert("o1");
    attrs0.insert("a2");
    objs2.insert("o2"); attrs2.insert("a2");
    objs4.insert("o2");
    attrs0.insert("a3");
    objs3.insert("o3"); attrs3.insert("a3");
    objs4.insert("o3");
    fca::concept con0(objs0,attrs0);
    fca::concept con1(objs1,attrs1);
    fca::concept con12(con1);
    fca::concept con2(objs2,attrs2);
    fca::concept con3(objs3,attrs3);
    fca::concept con4(objs4,attrs4);

    TS_ASSERT(con1 < con2);
    TS_ASSERT(!(con2 < con1));
    TS_ASSERT(!(con1 == con2));
    TS_ASSERT(con1 == con12);

    TS_ASSERT(con0 < con1);
    TS_ASSERT(con0 < con2);
    TS_ASSERT(con0 < con3);
    TS_ASSERT(con0 < con4);

    TS_ASSERT(!(con1 < con0));
    TS_ASSERT(con1 < con2);
    TS_ASSERT(con1 < con3);
    TS_ASSERT(con1 < con4);

    TS_ASSERT(!(con2 < con0));
    TS_ASSERT(!(con2 < con1));
    TS_ASSERT(!(con2 < con3)); //formally incomparable, but |objs3| < |objs2|
    TS_ASSERT(con2 < con4);

    TS_ASSERT(!(con3 < con0));
    TS_ASSERT(!(con3 < con1)); //formally incomparable, but "o1" < "o3"
    TS_ASSERT(con3 < con2);   //formally incomparable, but |objs3| < |objs2|
    TS_ASSERT(con3 < con4);

    //con4 is the top element
    TS_ASSERT(!(con4 < con0));
    TS_ASSERT(!(con4 < con1));
    TS_ASSERT(!(con4 < con2));
    TS_ASSERT(!(con4 < con3));

    TS_ASSERT(!(con0 == con1));
    TS_ASSERT(!(con0 == con2));
    TS_ASSERT(!(con0 == con3));
    TS_ASSERT(!(con0 == con4));
    TS_ASSERT(!(con1 == con0));
    TS_ASSERT(!(con1 == con2));
    TS_ASSERT(!(con1 == con3));
    TS_ASSERT(!(con1 == con4));
    TS_ASSERT(!(con2 == con1));
    TS_ASSERT(!(con2 == con0));
    TS_ASSERT(!(con2 == con3));
    TS_ASSERT(!(con2 == con4));
    TS_ASSERT(!(con3 == con1));
    TS_ASSERT(!(con3 == con2));
    TS_ASSERT(!(con3 == con0));
    TS_ASSERT(!(con3 == con4));
    TS_ASSERT(!(con4 == con1));
    TS_ASSERT(!(con4 == con2));
    TS_ASSERT(!(con4 == con3));
    TS_ASSERT(!(con4 == con0));
  }

  class vstor : public fca::concept_visitor {
  private:
    size_t num;
  public:
    vstor() : num(0) {}
    size_t getNum() const { return num; }
    void visit(const fca::concept& c) {
      num = c.getAttributes().size() + c.getObjects().size();
    }
  };

  class predicate : public fca::concept_predicate {
  public:
    bool pred (const fca::concept& c) {
      return (c.getObjects().size() > 1);
    }
  };

  void testAcceptor(void) {
    std::set<std::string> objs1, objs2;
    std::set<std::string> attrs1, attrs2;

    //want con1 < con2
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs2.insert("o2"); attrs2.insert("a2");
    fca::concept con1(objs1,attrs1);
    fca::concept con2(objs2,attrs2);

    vstor* numvisitor = new vstor();
    TS_ASSERT_EQUALS(numvisitor->getNum(),0);
    con1.accept(numvisitor);
    TS_ASSERT_EQUALS(numvisitor->getNum(),objs1.size()+attrs1.size());
    con2.accept(numvisitor);
    TS_ASSERT_EQUALS(numvisitor->getNum(),objs2.size()+attrs2.size());
    delete numvisitor;

    predicate* p = new predicate();
    TS_ASSERT(!con1.accept(p));
    TS_ASSERT(con2.accept(p));
    delete p;
  }

  void testDotWriter(void) {
    std::set<std::string> objs0, objs1, objs2, objs3, objs4;
    std::set<std::string> attrs0, attrs1, attrs2, attrs3, attrs4;

    //want con0 < con1 < con2 < con3
    attrs0.insert("a1");
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs4.insert("o1");
    attrs0.insert("a2");
    objs2.insert("o2"); attrs2.insert("a2");
    objs4.insert("o2");
    attrs0.insert("a3");
    objs3.insert("o3"); attrs3.insert("a3");
    objs4.insert("o3");
    fca::concept con0(objs0,attrs0);
    fca::concept con1(objs1,attrs1);
    fca::concept con12(con1);
    fca::concept con2(objs2,attrs2);
    fca::concept con3(objs3,attrs3);
    fca::concept con4(objs4,attrs4);

    std::ostringstream os0;
    fca::dot_writer* dw0 = new fca::dot_writer(os0);
    con0.write(dw0);
    TS_ASSERT_EQUALS(os0.str(),"\";a1, a2, a3\"");
    delete dw0;

    std::ostringstream os1;
    fca::dot_writer* dw1 = new fca::dot_writer(os1);
    con1.write(dw1);
    TS_ASSERT_EQUALS(os1.str(),"\"o1;a1\"");
    delete dw1;

    std::ostringstream os2;
    fca::dot_writer* dw2 = new fca::dot_writer(os2);
    con2.write(dw2);
    TS_ASSERT_EQUALS(os2.str(),"\"o1, o2;a1, a2\"");
    delete dw2;

    std::ostringstream os3;
    fca::dot_writer* dw3 = new fca::dot_writer(os3);
    con3.write(dw3);
    TS_ASSERT_EQUALS(os3.str(),"\"o3;a3\"");
    delete dw3;

    std::ostringstream os4;
    fca::dot_writer* dw4 = new fca::dot_writer(os4);
    con4.write(dw4);
    TS_ASSERT_EQUALS(os4.str(),"\"o1, o2, o3;\"");
    delete dw4;

  }

  void testAttrDotWriter(void) {
    std::set<std::string> objs0, objs1, objs2, objs3, objs4;
    std::set<std::string> attrs0, attrs1, attrs2, attrs3, attrs4;

    //want con0 < con1 < con2 < con3
    attrs0.insert("a1");
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs4.insert("o1");
    attrs0.insert("a2");
    objs2.insert("o2"); attrs2.insert("a2");
    objs4.insert("o2");
    attrs0.insert("a3");
    objs3.insert("o3"); attrs3.insert("a3");
    objs4.insert("o3");
    fca::concept con0(objs0,attrs0);
    fca::concept con1(objs1,attrs1);
    fca::concept con12(con1);
    fca::concept con2(objs2,attrs2);
    fca::concept con3(objs3,attrs3);
    fca::concept con4(objs4,attrs4);

    std::ostringstream os0;
    fca::attrcount_writer* dw0 = new fca::attrcount_writer(os0);
    con0.write(dw0);
    TS_ASSERT_EQUALS(os0.str(),"\";3\"");
    delete dw0;

    std::ostringstream os1;
    fca::attrcount_writer* dw1 = new fca::attrcount_writer(os1);
    con1.write(dw1);
    TS_ASSERT_EQUALS(os1.str(),"\"o1;1\"");
    delete dw1;

    std::ostringstream os2;
    fca::attrcount_writer* dw2 = new fca::attrcount_writer(os2);
    con2.write(dw2);
    TS_ASSERT_EQUALS(os2.str(),"\"o1, o2;2\"");
    delete dw2;

    std::ostringstream os3;
    fca::attrcount_writer* dw3 = new fca::attrcount_writer(os3);
    con3.write(dw3);
    TS_ASSERT_EQUALS(os3.str(),"\"o3;1\"");
    delete dw3;

    std::ostringstream os4;
    fca::attrcount_writer* dw4 = new fca::attrcount_writer(os4);
    con4.write(dw4);
    TS_ASSERT_EQUALS(os4.str(),"\"o1, o2, o3;0\"");
    delete dw4;

  }

  void testEntropyDotWriter(void) {
    std::set<std::string> objs0, objs1, objs2, objs3, objs4;
    std::set<std::string> attrs0, attrs1, attrs2, attrs3, attrs4;

    //want con0 < con1 < con2 < con3
    attrs0.insert("a1");
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs4.insert("o1");
    attrs0.insert("a2");
    objs2.insert("o2"); attrs2.insert("a2");
    objs4.insert("o2");
    attrs0.insert("a3");
    objs3.insert("o3"); attrs3.insert("a3");
    objs4.insert("o3");
    fca::concept con0(objs0,attrs0);
    fca::concept con1(objs1,attrs1);
    fca::concept con12(con1);
    fca::concept con2(objs2,attrs2);
    fca::concept con3(objs3,attrs3);
    fca::concept con4(objs4,attrs4);

    std::ostringstream os0;
    fca::entropy_writer* dw0 = new fca::entropy_writer(os0,3);
    con0.write(dw0);
    TS_ASSERT_EQUALS(os0.str(),"\";h=0\"");
    delete dw0;

    std::ostringstream os1;
    fca::entropy_writer* dw1 = new fca::entropy_writer(os1,3);
    con1.write(dw1);
    TS_ASSERT_EQUALS(os1.str(),"\"o1;h=0.528321\"");
    delete dw1;

    std::ostringstream os2;
    fca::entropy_writer* dw2 = new fca::entropy_writer(os2,3);
    con2.write(dw2);
    TS_ASSERT_EQUALS(os2.str(),"\"o1, o2;h=0.389975\"");
    delete dw2;

    std::ostringstream os3;
    fca::entropy_writer* dw3 = new fca::entropy_writer(os3,3);
    con3.write(dw3);
    TS_ASSERT_EQUALS(os3.str(),"\"o3;h=0.528321\"");
    delete dw3;

    std::ostringstream os4;
    fca::entropy_writer* dw4 = new fca::entropy_writer(os4,3);
    con4.write(dw4);
    TS_ASSERT_EQUALS(os4.str(),"\"o1, o2, o3;h=0\"");
    delete dw4;

  }

  void testCSVWriter(void) {
    std::set<std::string> objs0, objs1, objs2, objs3, objs4;
    std::set<std::string> attrs0, attrs1, attrs2, attrs3, attrs4;

    //want con0 < con1 < con2 < con3
    attrs0.insert("a1");
    objs1.insert("o1"); attrs1.insert("a1");
    objs2.insert("o1"); attrs2.insert("a1");
    objs4.insert("o1");
    attrs0.insert("a2");
    objs2.insert("o2"); attrs2.insert("a2");
    objs4.insert("o2");
    attrs0.insert("a3");
    objs3.insert("o3"); attrs3.insert("a3");
    objs4.insert("o3");
    fca::concept con0(objs0,attrs0);
    fca::concept con1(objs1,attrs1);
    fca::concept con12(con1);
    fca::concept con2(objs2,attrs2);
    fca::concept con3(objs3,attrs3);
    fca::concept con4(objs4,attrs4);

    std::ostringstream os0;
    fca::csv_writer* dw0 = new fca::csv_writer(os0,3);
    con0.write(dw0);
    TS_ASSERT_EQUALS(os0.str(),"\"\",\"a1, a2, a3\",3,0");
    delete dw0;

    std::ostringstream os1;
    fca::csv_writer* dw1 = new fca::csv_writer(os1,3);
    con1.write(dw1);
    TS_ASSERT_EQUALS(os1.str(),"\"o1\",\"a1\",1,0.528321");
    delete dw1;

    std::ostringstream os2;
    fca::csv_writer* dw2 = new fca::csv_writer(os2,3);
    con2.write(dw2);
    TS_ASSERT_EQUALS(os2.str(),"\"o1, o2\",\"a1, a2\",2,0.389975");
    delete dw2;

    std::ostringstream os3;
    fca::csv_writer* dw3 = new fca::csv_writer(os3,3);
    con3.write(dw3);
    TS_ASSERT_EQUALS(os3.str(),"\"o3\",\"a3\",1,0.528321");
    delete dw3;

    std::ostringstream os4;
    fca::csv_writer* dw4 = new fca::csv_writer(os4,3);
    con4.write(dw4);
    TS_ASSERT_EQUALS(os4.str(),"\"o1, o2, o3\",\"\",0,0");
    delete dw4;

  }
};

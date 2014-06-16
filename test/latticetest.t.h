#include <cxxtest/TestSuite.h>

#include "concept.hpp"
#include "lattice.hpp"

//Note: doesn't touch uparc_visitor

class LatticeTests: public CxxTest::TestSuite {
public:
  void testEmptyWrite(void) {
    fca::lattice lat;
    std::ostringstream dos;
    fca::concept_writer_ptr w(new fca::dot_writer(dos));
    fca::writeLattice(lat,w);

    TS_ASSERT_EQUALS(dos.str(),"digraph lattice {\n}\n");
  }

  void testReadWriteAcceptor(void) {
    std::ostringstream oss;
    oss << "digraph lattice {\n";
    oss << "\"o1,o2,o3;\"->\"o1,o2;a1\"\n";
    oss << "\"o1,o2,o3;\"->\"o3;a3\"\n";
    oss << "\"o1,o2;a1\"->\"o2;a1,a2\"\n";
    oss << "\"o3;a3\"->\";a1,a2,a3\"\n";
    oss << "\"o2;a1,a2\"->\";a1,a2,a3\"\n";
    oss << "}";
    std::string input = oss.str();
    std::istringstream iss(input);

    fca::lattice lat;
    fca::arc_visitor_ptr v(new fca::downarc_visitor(lat));
    fca::readDotLattice(iss,v);

    std::ostringstream dos;
    fca::concept_writer_ptr w(new fca::dot_writer(dos));
    fca::writeLattice(lat,w);

    std::ostringstream exp;
    exp << "digraph lattice {\n";
    exp << "\"o1, o2, o3;\"->\"o3;a3\"\n";
    exp << "\"o1, o2, o3;\"->\"o1, o2;a1\"\n";
    exp << "\"o1, o2;a1\"->\"o2;a1, a2\"\n";
    exp << "\"o3;a3\"->\";a1, a2, a3\"\n";
    exp << "\"o2;a1, a2\"->\";a1, a2, a3\"\n";
    exp << "}\n";
    std::string expected = exp.str();
    TS_ASSERT_EQUALS(dos.str(),expected);
  }

  void testLocalAcceptor(void) {
    std::ostringstream oss;
    oss << "digraph lattice {\n";
    oss << "\"o1,o2,o3;\"->\"o1,o2;a1\"\n";
    oss << "\"o1,o2,o3;\"->\"o3;a3\"\n";
    oss << "\"o1,o2;a1\"->\"o2;a1,a2\"\n";
    oss << "\"o3;a3\"->\";a1,a2,a3\"\n";
    oss << "\"o2;a1,a2\"->\";a1,a2,a3\"\n";
    oss << "}";
    std::string input = oss.str();
    std::istringstream iss(input);

    fca::lattice lat;
    fca::arc_visitor_ptr v(new fca::downarc_visitor(lat));
    fca::readDotLattice(iss,v);

    std::set<std::string> objs1, attrs1;
    objs1.insert("o1");
    objs1.insert("o2");
    objs1.insert("o3");
    fca::concept con(objs1,attrs1);

    std::ostringstream dos;
    fca::concept_writer_ptr dw(new fca::dot_writer(dos));
    fca::arc_writer_ptr aw(new fca::arc_writer(dw));
    lat.accept(con,aw);

    std::string expected = "\"o1, o2, o3;\"->\"o3;a3\"\n\"o1, o2, o3;\"->\"o1, o2;a1\"\n";
    TS_ASSERT_EQUALS(dos.str(),expected);
  }

};

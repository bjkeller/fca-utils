//
//  concept.h
//
//
//  Created by Benjamin Keller on 5/9/12.
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
//

#ifndef _concept_h
#define _concept_h

#include <iostream>
#include <string>
#include <set>

namespace fca {

//concepts
//
class concept {
public:
  concept() : objects(), attributes() {}
  concept(const std::set<std::string>& objs, const std::set<std::string>& attrs) : objects(objs), attributes(attrs) {}
  concept(const concept& con) : objects(con.objects), attributes(con.attributes) {}

  concept& operator= (const concept& con);

  bool operator== (const concept& con) {
    return objects == con.objects;
  }

  bool operator< (const concept& con) const;

  const std::set<std::string>& getObjects() const { return objects; }
  const std::set<std::string>& getAttributes() const { return attributes; }

  double entropy(std::size_t count) const;

  void accept(class concept_visitor* v) const;
  bool accept(class concept_predicate* v) const;
  void write(class concept_writer* v) const;

private:
  std::set<std::string> objects;
  std::set<std::string> attributes;
};

/*
class concept_writer {
public:
  void write(ostream& os, const concept& con) const;
  virtual void writeObjects(ostream& os, const concept& con) const;
  virtual void writeAttributes(ostream& os, const concept& con) const;
};

void writeConcept(ostream& os, const concept& con);
void writeConceptDiff(ostream& os, const concept& con, const set<string>& diffset);
void writeConceptAttrCount(ostream& os, const concept& con);
*/

class concept_visitor {
 public:
  virtual void visit(const concept&) = 0;
};

class concept_predicate {
 public:
  virtual bool pred (const concept&) = 0;
};

class concept_writer {
 public:
  concept_writer(std::ostream& out) : os(out) {}
  virtual void write(const concept&) const;
  virtual void writePrefix() const {}
  virtual void writeSuffix() const {}
  virtual void writeSeparator() const {}
  virtual void writeSet(const std::set<std::string>&) const;
  virtual void writeObjects(const concept&) const;
  virtual void writeAttributes(const concept&) const;
  std::ostream& getStream() const { return os; }
protected:
  std::ostream& os;
};

class dot_writer : public concept_writer {
 public:
   dot_writer(std::ostream& out) : concept_writer(out) {}
  /*virtual*/ void writePrefix() { os << "\""; }
  /*virtual*/ void writeSeparator() { os << ";"; }
  /*virtual*/ void writeSuffix() { os << "\""; }
};

class attrcount_writer : public dot_writer {
public:
  attrcount_writer(std::ostream& out) : dot_writer(out) {}
  /* virtual */ void writeAttributes(const concept&) const;
};

class entropy_writer : public dot_writer {
 public:
  entropy_writer(std::ostream& out,std::size_t cnt) : dot_writer(out), attrcnt(cnt) {}
  /* virtual */ void writeAttributes(const concept&) const;
 private:
  std::size_t attrcnt;
};

class csv_writer : public concept_writer {
 public:
  csv_writer(std::ostream& out, std::size_t cnt) : concept_writer(out), attrcnt(cnt) {}
  /*virtual*/ void writeSeparator() { os << ","; }
  /*virtual*/ void writeSet( const std::set<std::string>&) const;
  /*virtual*/ void writeAttributes( const concept&) const;
 private:
  std::size_t attrcnt;
};

}; //end namespace fca
#endif
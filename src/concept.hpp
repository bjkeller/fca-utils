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

#include <boost/shared_ptr.hpp>

namespace fca {

class concept_visitor;
typedef boost::shared_ptr<concept_visitor> concept_visitor_ptr;

class concept_predicate;
typedef boost::shared_ptr<concept_predicate> concept_pred_ptr;

class concept_writer;
typedef boost::shared_ptr<concept_writer> concept_writer_ptr;

/*
 *  concepts
 *
 * Implements a pair of sets that can be used to represent formal concepts, however
 * does not enforce closure property since not linked to formal context other than
 * in comparison operators.
 *
 */
class concept {
public:
  concept() : objects(), attributes() {}
  concept(const std::set<std::string>& objs, const std::set<std::string>& attrs) : objects(objs), attributes(attrs) {}
  concept(const concept& con) : objects(con.objects), attributes(con.attributes) {}

  concept& operator= (const concept& con);

  bool operator== (const concept& con) const {
    return objects == con.objects;
  }

  bool operator< (const concept& con) const;

  const std::set<std::string>& getObjects() const { return objects; }
  const std::set<std::string>& getAttributes() const { return attributes; }

  double attrEntropy(std::size_t count) const;
  double objEntropy(std::size_t count) const;

  void accept(concept_visitor_ptr) const;
  bool accept(concept_pred_ptr) const;
  void write(concept_writer_ptr) const;

private:
  std::set<std::string> objects;
  std::set<std::string> attributes;
};

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
  /*virtual*/ void writePrefix() const { os << "\""; }
  /*virtual*/ void writeSeparator() const { os << ";"; }
  /*virtual*/ void writeSuffix() const { os << "\""; }
};

class attrcount_writer : public dot_writer {
public:
  attrcount_writer(std::ostream& out) : dot_writer(out) {}
  /* virtual */ void writeAttributes(const concept&) const;
};

class objcount_writer : public dot_writer {
public:
  objcount_writer(std::ostream& out) : dot_writer(out) {}
  /* virtual */ void writeObjects(const concept&) const;
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
  /*virtual*/ void writeSeparator() const { os << ","; }
  /*virtual*/ void writeSet( const std::set<std::string>&) const;
  /*virtual*/ void writeAttributes( const concept&) const;
 private:
  std::size_t attrcnt;
};

}; //end namespace fca
#endif

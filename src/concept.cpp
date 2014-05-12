//
//  concept.cpp
//
//
//  Created by Benjamin Keller on 5/9/12.
//  Copyright (c) 2012 Benjamin Keller. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <cmath>

#include "concept.hpp"

namespace fca {

concept& concept::operator= (const concept& con) {
    if (this != &con) {
        objects = con.objects;
        attributes = con.attributes;
    }
    return *this;
}

bool concept::operator< (const concept& con) const {
    if (objects.size() == con.objects.size()) {
        std::set<std::string>::const_iterator o_i = objects.begin();
        std::set<std::string>::const_iterator c_i = con.objects.begin();
        while (o_i != objects.end() && c_i != con.objects.end() && (*o_i) == (*c_i)) {
            o_i++;
            c_i++;
        }
        return o_i != objects.end() && c_i != objects.end() && (*o_i) < (*c_i);
    }
    return objects.size() < con.objects.size();
}


double concept::entropy(size_t attrtot) const {
    double entropy = 0.0;
    if (!attributes.empty() && attributes.size() != attrtot) {
      double p_con = attributes.size() / static_cast<double>(attrtot);
      double log_p_con = std::log2(p_con);
      entropy = -1*(p_con*log_p_con);
    }
    return entropy;
}

void concept::accept(concept_visitor_ptr v) const { v->visit(*this); }
bool concept::accept(concept_pred_ptr v) const { return v->pred(*this); }
void concept::write(concept_writer_ptr v) const { v->write(*this); }

void concept_writer::write(const concept& con) const {
  this->writePrefix();
  this->writeObjects(con);
  this->writeSeparator();
  this->writeAttributes(con);
  this->writeSuffix();
}

void concept_writer::writeSet(const std::set<std::string>& s) const {
    std::set<std::string>::const_iterator s_i = s.begin();
    while (s_i != s.end()) {
        os << *s_i++;
        if (s_i != s.end()) {
            os << ", ";
        }
    }
}

void concept_writer::writeObjects(const concept& con) const {
  this->writeSet(con.getObjects());
}

void concept_writer::writeAttributes(const concept& con) const {
  this->writeSet(con.getAttributes());
}

void attrcount_writer::writeAttributes(const concept& con) const {
  os << con.getAttributes().size();
}

void entropy_writer::writeAttributes(const concept& con) const {
  os << "h=" << con.entropy(attrcnt);
}

void csv_writer::writeSet(const std::set<std::string>& s) const {
  os << "\"";
  concept_writer::writeSet(s);
  os << "\"";
}

void csv_writer::writeAttributes(const concept& con) const {
  concept_writer::writeAttributes(con);
  os << "," << con.getAttributes().size() << ",";
  os << con.entropy(attrcnt);
}

/*
void writeSet(ostream& os, const set<string>& s) {
    set<string>::const_iterator s_i = s.begin();
    while (s_i != s.end()) {
        os << *s_i++;
        if (s_i != s.end()) {
            os << ", ";
        }
    }
}

void writeDiffSet(ostream& os, const set<string>& s, const set<string>& ds) {
    set<string>::const_iterator s_i = s.begin();
    while (s_i != s.end() && ds.find(*s_i) != ds.end()) {
        s_i++;
    }
    if (s_i != s.end()) {
        os << *s_i++;
        while (s_i != s.end()) {
            if (ds.find(*s_i) == ds.end()) {
                os << ", " << *s_i;
            }
            s_i++;

        }
    }
}
void writeConcept(ostream& os, const concept& con) {
    os << "\"";
    writeSet(os, con.getObjects());
    os << ";";
    writeSet(os, con.getAttributes());
    os << "\"";
}

void writeConceptDiff(ostream& os, const concept& con, const set<string>& diffset) {
    os << "\"";
    writeSet(os, con.getObjects());
    os << ";";
    writeDiffSet(os, con.getAttributes(), diffset);
    os << "\"";
}

void writeConceptAttrCount(ostream& os, const concept& con) {
    os << "\"";
    writeSet(os, con.getObjects());
    os << "; ";
    os << con.getAttributes().size();
    os << "\"";
}

void concept_writer::write(ostream& os, const concept& con) const {
    os << "\"";
    this->writeObjects(os,con);
    os << ";";
    this->writeAttributes(os,con);
    os << "\"";
}

void concept_writer::writeObjects(ostream& os, const concept& con) const {
  writeSet(os, con.getObjects());
}

void concept_writer::writeAttributes(ostream& os, const concept& con) const {
  writeSet(os, con.getAttributes());
}
*/

}; //end namespace fca

/*
 * Polyhedron_scan_coff.h
 *
 *  Created on: Jan 16, 2017
 *      Author: andrea
 */

#ifndef SEMANTICMESHRECONSTRUCTION_POLYHEDRON_SCAN_COFF_H_
#define SEMANTICMESHRECONSTRUCTION_POLYHEDRON_SCAN_COFF_H_

#include <types.hpp>
#include <CGAL/basic.h>
#include <CGAL/IO/File_header_OFF.h>
#include <CGAL/IO/File_scanner_OFF.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <iostream>
#include <cstddef>

typedef Polyhedronmy::HalfedgeDS HDS;

class Polyhedron_scan_COFF : public CGAL::Modifier_base<HDS> {
protected:
  std::istream& m_in;
public:

  typedef HDS Halfedge_data_structure;

// DEFINITION
//
// Polyhedron_scan_COFF<Traits> is a polyhedral surface builder.
// It scans a polyhedron given in OFF from a stream and appends it
// incrementally using the incremental builder.

  Polyhedron_scan_COFF(std::istream& in) :
      m_in(in) {
  }

  // Activation
  void operator()(HDS& hds);

};


#endif /* SEMANTICMESHRECONSTRUCTION_POLYHEDRON_SCAN_COFF_H_ */

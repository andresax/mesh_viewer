
#include <Polyhedron_scan_COFF.h>


void Polyhedron_scan_COFF::operator()(HDS& target) {
  CGAL::File_scanner_OFF scanner(m_in);
  if (!m_in) {
    if (scanner.verbose()) {
      std::cerr << " " << std::endl;
      std::cerr << "Polyhedron_scan_COFF<HDS>::" << std::endl;
      std::cerr << "operator(): input error: file format is not in "
          "OFF." << std::endl;
    }
    return;
  }

  CGAL::Polyhedron_incremental_builder_3 <HDS> B(target, scanner.verbose());
  B.begin_surface(scanner.size_of_vertices(), scanner.size_of_facets(), scanner.size_of_halfedges());


  std::vector<CGAL::Color> cVector;
  // read in all vertices
  std::size_t i;
  for (i = 0; i < scanner.size_of_vertices(); i++) {
    HDS::Traits::Point_3 p;
    file_scan_vertex(scanner, p);
    B.add_vertex(CGAL::Polyhedron_incremental_builder_3 <HDS>::Point_3(p.x(),p.y(),p.z()));
    if (scanner.has_colors()) {
      CGAL::Color c;
      file_scan_color(scanner, c);
      cVector.push_back(c);
    } else
      scanner.skip_to_next_vertex(i);
  }
  if (!m_in || B.error()) {
    B.rollback();
    m_in.clear(std::ios::badbit);
    return;
  }

  // read in all facets
  for (i = 0; i < scanner.size_of_facets(); i++) {
    B.begin_facet();
    std::size_t no;
    scanner.scan_facet(no, i);
    if (!m_in || B.error() || no < 3) {
      if (scanner.verbose()) {
        std::cerr << " " << std::endl;
        std::cerr << "Polyhedron_scan_COFF<Traits>::" << std::endl;
        std::cerr << "operator()(): input error: facet " << i << " has less than 3 vertices." << std::endl;
      }
      B.rollback();
      m_in.clear(std::ios::badbit);
      return;
    }

    std::vector<CGAL::Color> temp;
    for (std::size_t j = 0; j < no; j++) {
      std::size_t index;
      scanner.scan_facet_vertex_index(index, i);
      B.add_vertex_to_facet(index);
      temp.push_back(cVector[index]);

    }
    //TO DO : Insert read color

    int maxOcc=0;
    CGAL::Color max;
    for(auto col:temp){
      if(std::count(temp.begin(), temp.end(),col) > maxOcc){
        max = col;
        maxOcc = std::count(temp.begin(), temp.end(),col);
      }
    }


    Halfedge_handlemy curH = B.end_facet();
    Facet_handlemy fh =curH->facet();
    fh->color = max;
    fh->count=maxOcc;
    scanner.skip_to_next_facet(i);
  }
  if (!m_in || B.error()) {
    B.rollback();
    m_in.clear(std::ios::badbit);
    return;
  }
  if (B.check_unconnected_vertices()) {
    if (!B.remove_unconnected_vertices()) {
      if (scanner.verbose()) {
        std::cerr << " " << std::endl;
        std::cerr << "Polyhedron_scan_COFF<Traits>::" << std::endl;
        std::cerr << "operator()(): input error: cannot "
            "successfully remove isolated vertices." << std::endl;
      }
      B.rollback();
      m_in.clear(std::ios::badbit);
      return;
    }
  }
  B.end_surface();
}

#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <string>
#include <opencv2/core/core.hpp>

#include <glm.hpp>
#include <Eigen/Core>
#include <glm.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_hierarchy_3.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/intersections.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Color.h>
#include <CGAL/algorithm.h>
//This header includes some useful data structure to manage the configuration parameters

struct CameraType {
    glm::mat3 intrinsics;
    glm::mat3 rotation;
    glm::vec3 translation;
    glm::mat4 cameraMatrix;
    glm::mat4 mvp;
    glm::vec3 center;
    std::string cameraPath;
    int imageWidth;
    int imageHeight;
    glm::vec3 distortion_coeff;
};


struct SfMData {
  int numCameras_;
  std::vector<CameraType> camerasList_;
  int imageWidth_, imageHeight_;
};

struct ViewerConfiguration {
    std::string camerasFilePath_;
    std::string correspondencesPath_;
    std::string pathOutDir_;
} ;





enum class classLabel {
  CAR, WALL, GROUND, VEGETATION, POLE,PEDESTRIAN, UNDEFINED,
};
template<class T>
class ClassVote {
public:
  ClassVote() {

    classVote[classLabel::CAR] = static_cast<T>(0);
    classVote[classLabel::GROUND] = static_cast<T>(0);
    classVote[classLabel::POLE] = static_cast<T>(0);
    classVote[classLabel::PEDESTRIAN] = static_cast<T>(0);
    classVote[classLabel::UNDEFINED] = static_cast<T>(0);
    classVote[classLabel::VEGETATION] = static_cast<T>(0);
    classVote[classLabel::WALL] = static_cast<T>(0);
  }
  T maxValue() const {
    double maxval = -1;
    for (auto it = classVote.begin(); it != classVote.end(); it++) {
      if (maxval < it->second) {
        maxval = it->second;
      }
    }
    return maxval;
  }
  classLabel maxClass()const{
    classLabel max;
    double maxval = -1;
    for (auto it = classVote.begin(); it != classVote.end(); it++) {
      if (maxval < it->second) {
        maxval = it->second;
        max = it->first;
      }
    }
    return max;
  }
  ClassVote& operator=(ClassVote other) {
    auto iteratorInput = other.classVote.begin();
    for (auto iterator = this->classVote.begin(); iterator != this->classVote.end() && iteratorInput != other.classVote.end(); iterator++, iteratorInput++) {
      iterator->second = iteratorInput->second;
    }
    return *this;
  }
  std::map<classLabel, T> classVote;
};


class LabelClass {
public:
  LabelClass() :
      label(classLabel::UNDEFINED), id(0) {
  }
  LabelClass(classLabel l) :
      label(l), id(0) {
  }
  LabelClass(classLabel l, size_t i) :
      label(l), id(i) {
  }

  classLabel l() const {
    return label;
  }

  size_t idx() const {
    return id;
  }

  void set_label(classLabel l) {
    label = l;
  }
  void set_id(size_t v) {
    id = v;
  }
  bool operator==(const LabelClass &lab) const {
    return l() == lab.l() && idx() == lab.idx();
  }

  bool operator!=(const LabelClass &l) const {
    return !((*this) == l);
  }

  LabelClass& operator=(const LabelClass &l) {
    label = l.l();
    id = l.idx();
    return *this;
  }

private:
  classLabel label;
  size_t id;
};



// A face type with a color member variable.
template<class Refs>
struct My_face : public CGAL::HalfedgeDS_face_base<Refs> {
  CGAL::Color color;
  int count=0;
  LabelClass label;
};
// An items type using my face.
struct My_items : public CGAL::Polyhedron_items_3 {
  template<class Refs, class Traits>
  struct Face_wrapper {
    typedef My_face<Refs> Face;
  };
};





typedef CGAL::Simple_cartesian<float> Kernelmy;
typedef CGAL::Polyhedron_3<Kernelmy, My_items> Polyhedronmy;
typedef Polyhedronmy::Facet_handle Facet_handlemy;
typedef Polyhedronmy::Halfedge_handle Halfedge_handlemy;
typedef Polyhedronmy::Facet_iterator Facet_iteratormy;
typedef Polyhedronmy::Vertex_handle Vertex_handlemy;
typedef Polyhedronmy::Halfedge_iterator Halfedge_iteratormy;
typedef Polyhedronmy::Point_3 Pointmy;
typedef Kernelmy::Vector_3 Vectormy;













#endif /* TYPES_HPP_ */

#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <string>
#include <opencv2/core/core.hpp>

#include <glm.hpp>
#include <Eigen/Core>
//This header includes some useful data structure to manage the configuration parameters

struct CameraType {
    glm::mat3 intrinsics;
    glm::mat3 rotation;
    glm::vec3 translation;
    glm::mat4 cameraMatrix;
    //camera center
    glm::vec3 center;
};


struct SfMData {

  int numPoints_;
  int numCameras_;

  std::vector<glm::vec3> points_;
  std::vector<CameraType> camerasList_;
  std::vector<std::string> camerasPaths_;

  std::vector<std::vector<int> > camViewingPointN_;
  std::vector<std::vector<int> > pointsVisibleFromCamN_;
  std::vector<std::vector<glm::vec2> > point2DoncamViewingPoint_;

  int imageWidth_, imageHeight_;
};

typedef struct Config {
    std::string camerasFilePath_;
    std::string correspondencesPath_;
    std::string pathOutDir_;
} ViewerConfiguration;

#endif /* TYPES_HPP_ */

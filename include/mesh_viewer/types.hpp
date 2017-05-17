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

#endif /* TYPES_HPP_ */

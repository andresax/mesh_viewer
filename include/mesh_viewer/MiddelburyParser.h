#ifndef MiddelburyParser_H_
#define MiddelburyParser_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "types.hpp"

#include <glm.hpp>

class MiddelburyParser {
  public:
    MiddelburyParser(std::string fileInput);
    virtual ~MiddelburyParser();
    bool parseFile(const int &downsample = 1);

    const std::vector<CameraType>& getCamerasList() const {
      return camerasList_;
    }

    int getNumCameras() const {
      return numCameras_;
    }

    int getNumPoints() const {
      return numPoints_;
    }

  const std::vector<std::string>& getCamerasPaths() const {
    return camerasPaths_;
  }

  protected:
    std::string fileName_;
    std::ifstream fileStream_;
    int numCameras_;
    int numPoints_;
    std::vector<CameraType> camerasList_;
    std::vector<std::string> camerasPaths_;
};
#endif /* MiddelburyParser_H_ */

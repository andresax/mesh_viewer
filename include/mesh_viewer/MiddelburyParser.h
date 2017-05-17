#ifndef MiddelburyParser_H_
#define MiddelburyParser_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <types.hpp>

#include <glm.hpp>

class MiddelburyParser{
  public:
    MiddelburyParser(std::string fileInput);
    virtual ~MiddelburyParser();
    bool parse();
  const SfMData& getSfmData() const {
    return sfm_data_;
  }

    const std::vector<CameraType>& getCamerasList() const {
      return camerasList_;
    }

    int getNumCameras() const {
      return camerasList_.size();
    }
  protected:
    std::string fileName_;
    std::ifstream fileStream_;
    int numCameras_;
    std::vector<CameraType> camerasList_;
    SfMData sfm_data_;
};
#endif /* MiddelburyParser_H_ */

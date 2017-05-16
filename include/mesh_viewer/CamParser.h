#ifndef CAMPARSER_H_
#define CAMPARSER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <types.hpp>

#include <glm.hpp>

class CamParser {
  public:
    CamParser(std::string fileInput);
    virtual ~CamParser();
    virtual bool parseFile()=0;

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
};
#endif /* CAMPARSER_H_ */

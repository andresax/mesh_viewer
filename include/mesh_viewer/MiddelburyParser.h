#ifndef MiddelburyParser_H_
#define MiddelburyParser_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <CameraParser.h>
#include <types.hpp>

#include <glm.hpp>

class MiddelburyParser :CameraParser{
  public:
    MiddelburyParser(std::string fileInput);
    virtual ~MiddelburyParser();
    bool parseFile();

  protected:
    std::string fileName_;
    std::ifstream fileStream_;
    int numCameras_;
    int numPoints_;
    std::vector<CameraType> camerasList_;
};
#endif /* MiddelburyParser_H_ */

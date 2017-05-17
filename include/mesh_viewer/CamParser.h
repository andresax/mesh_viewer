#ifndef CAMPARSER_H_
#define CAMPARSER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <types.hpp>

#include <OpenMvgParser.h>
#include <MiddelburyParser.h>

#include <glm.hpp>

class CamParser {
  public:
    CamParser(std::string fileInput);
    virtual ~CamParser();
    bool parseFile();

    const SfMData& getSfmData() const {
    return sfm_data_;
  }


  protected:
    std::string fileName_;
    std::ifstream fileStream_;
    SfMData sfm_data_;

};
#endif /* CAMPARSER_H_ */

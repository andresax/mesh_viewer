/*
 * OpenMvgParser.h
 *
 *  Created on: 16 mar 2016
 *      Author: andrea
 */

#ifndef CAM_PARSERS_OPENMVGPARSER_H_
#define CAM_PARSERS_OPENMVGPARSER_H_

#include <types.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <rapidjson/document.h>


class OpenMvgParser {
public:
  OpenMvgParser(std::string path);
  virtual ~OpenMvgParser();
  bool parseFile();

  const SfMData& getSfmData() const {
    return sfm_data_;
  }
    const std::vector<CameraType>& getCamerasList() const {
      return camerasList_;
    }

    int getNumCameras() const {
      return camerasList_.size();
    }

private:
  void parseViews(const std::map<int,glm::mat3> & intrinsics, const std::map<int, glm::vec3> &distortion, const std::map<int,CameraType> & extrinsics);
  void parseIntrinsics(std::map<int,glm::mat3> & intrinsics, std::map<int, glm::vec3> &distortion);
  void parseExtrinsics(std::map<int,CameraType> & extrinsics);


    std::string fileName_;
    std::ifstream fileStream_;
    int numCameras_;
    std::vector<CameraType> camerasList_;
  rapidjson::Document document_;
  SfMData sfm_data_;

};

#endif /* CAM_PARSERS_OPENMVGPARSER_H_ */

#include <ViewingDataParser.h>

#include <sstream>
#include <iostream>
ViewingDataParser::ViewingDataParser(std::string nameFile) {
  fileStream_.open(nameFile.c_str(),std::ios::in);
}

ViewingDataParser::~ViewingDataParser() {
}



bool ViewingDataParser::parseFile(){


  std::string line;

  //read num cam
  while(std::getline(fileStream_, line)){
    std::istringstream iss(line);
    ViewingTriplet temp;
    std::string pathS;
    iss >>temp.cameraId >> temp.imageId >> pathS;
    temp.meshPath = pathS;
    orderedViewingTriplets_.push_back(temp);
  }
}
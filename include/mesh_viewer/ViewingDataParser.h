#ifndef VIEWINGDATAPARSER
#define VIEWINGDATAPARSER

#include <iostream>
#include <fstream>
#include <vector>
#include <types.hpp>

#include <glm.hpp>

struct ViewingTriplet{
  int cameraId;
  std::string meshPath;
  int imageId = -1;
} ;


class ViewingDataParser {
  public:
    ViewingDataParser(std::string fileInput);
    virtual ~ViewingDataParser();
    bool parseFile();
    const std::vector<ViewingTriplet>& getOrderedViewingTriplets() const {
      return orderedViewingTriplets_;
    }
  private:
    std::string fileName_;
    std::ifstream fileStream_;
    std::vector<ViewingTriplet> orderedViewingTriplets_;
};
#endif /* VIEWINGDATAPARSER */

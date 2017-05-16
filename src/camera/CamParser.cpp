

CamParser::CamParser(std::string fileName) : fileName_(fileName), numCameras_(0), numPoints_(0) {
  fileStream_.open(fileName_.c_str(),std::ios::in);
}

CamParser::~CamParser() {
}

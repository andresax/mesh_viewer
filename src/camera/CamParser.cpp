
#include <OpenMvgParser.h>
#include <MiddelburyParser.h>

CamParser::CamParser(std::string fileName) : fileName_(fileName), numCameras_(0), numPoints_(0) {
  fileStream_.open(fileName_.c_str(),std::ios::in);
}

CamParser::~CamParser() {
}

bool CamParser::parseFile();
{
    if (fileName_.substr(fileName_.end()-9,8).equals("_par.txt")){
        MiddelburyParser mp(fileName_);
        mp.parse();
        sfm_data_ = mp.getSfmData();
    }else if(fileName_.substr(fileName_.end()-5,4).equals("json")){
        OpenMvgParser omvg(fileName_);
        omvg.parse();
        sfm_data_ = omvg.getSfmData();
    }
}
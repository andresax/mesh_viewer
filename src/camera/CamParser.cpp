
#include <OpenMvgParser.h>
#include <CamParser.h>
#include <MiddelburyParser.h>
#include <string>

CamParser::CamParser(std::string fileName) : fileName_(fileName) {
  fileStream_.open(fileName_.c_str(),std::ios::in);
}

CamParser::~CamParser() {
}

bool CamParser::parseFile()
{
    if (fileName_.substr(fileName_.size()-9,8).compare("_par.txt")==0){
        MiddelburyParser mp(fileName_);
        mp.parse();
        sfm_data_ = mp.getSfmData();
    }else if(fileName_.substr(fileName_.size()-5,4).compare("json")==0){
        OpenMvgParser omvg(fileName_);
        omvg.parse();
        sfm_data_ = omvg.getSfmData();
    }
    return true;
}
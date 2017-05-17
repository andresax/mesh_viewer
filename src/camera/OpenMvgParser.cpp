#include <OpenMvgParser.h>
#include <Exceptions.hpp>
#include <stdexcept>
#include <rapidjson/reader.h>
#include <utilities.hpp>

OpenMvgParser::OpenMvgParser(std::string fileName) {
  fileStream_.open(path.c_str());
}

OpenMvgParser::~OpenMvgParser() {
  sfm_data_.points_.clear();
  sfm_data_.camerasList_.clear();
  sfm_data_.camerasPaths_.clear();
  sfm_data_.camViewingPointN_.clear();
  sfm_data_.pointsVisibleFromCamN_.clear();
  sfm_data_.point2DoncamViewingPoint_.clear();
}

void OpenMvgParser::parse() {
  std::string str((std::istreambuf_iterator<char>(fileStream_)), std::istreambuf_iterator<char>());
  document_.Parse(str.c_str());

  try {
    if (!document_.IsObject())
      throw JsonParseException("JsonParseException--> the json file " + fileName_ + " is not valid");
  } catch (JsonParseException& e) {
    std::cerr << e.what() << std::endl;
    std::cout << e.what() << std::endl;
  }

  std::map<int, glm::mat3> intrinsics;
  std::map<int, glm::vec3> distortion;
  std::map<int, CameraType> extrinsics;

  parseIntrinsics(intrinsics,distortion);
  parseExtrinsics(extrinsics);

  parseViews(intrinsics,distortion, extrinsics);

  for (rapidjson::SizeType curCam = 0; curCam < sfm_data_.camerasList_.size(); curCam++) {
    utilities::convertToMvp2(sfm_data_.camerasList_[curCam],sfm_data_.camerasList_[curCam].mvp);
  }

  camerasList_=sfm_data_.camerasList_;
  return true;
}

void OpenMvgParser::parseViews(const std::map<int, glm::mat3> & intrinsics, const std::map<int, glm::vec3> &distortion, const std::map<int, CameraType> & extrinsics) {

  std::string basePath(document_["root_path"].GetString());

  try {
    if (!document_.HasMember("views"))
      throw JsonAccessException("JsonAccessException--> error while querying HasMember(views)");
    const rapidjson::Value& camerasJson = document_["views"];
    sfm_data_.numCameras_ = camerasJson.Size();
    sfm_data_.camerasList_.assign(sfm_data_.numCameras_, CameraType());
    sfm_data_.camerasPaths_.assign(sfm_data_.numCameras_, std::string());
    for (rapidjson::SizeType curCam = 0; curCam < camerasJson.Size(); curCam++) {

      std::string local(camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["local_path"].GetString());
      std::string filename(camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["filename"].GetString());

      sfm_data_.camerasList_[curCam].pathImage = basePath + local + filename;
      sfm_data_.camerasPaths_[curCam] = basePath + local + filename;

      sfm_data_.camerasList_[curCam].imageWidth = camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["width"].GetInt();
      sfm_data_.camerasList_[curCam].imageHeight = camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["height"].GetInt();
      int idIntrinsics = camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["id_intrinsic"].GetInt();
      int idExtrinsics = camerasJson[curCam]["value"]["ptr_wrapper"]["data"]["id_pose"].GetInt();

      try {
        glm::mat3 intr = intrinsics.at(idIntrinsics);
        glm::vec3 dist = distortion.at(idIntrinsics);

        sfm_data_.camerasList_[curCam].intrinsics = intr;
        sfm_data_.camerasList_[curCam].distortion_coeff = dist;
      } catch (std::out_of_range e) {
        std::cerr << "std::out_of_range exception trying to look for intrinsics matrix " << idIntrinsics << std::endl;
      }
      try {
        CameraType camCurrent = extrinsics.at(idExtrinsics);
        glm::mat4 eMatrix(0.0), kMatrix(0.0);
        for (int curR = 0; curR < 3; curR++) {
          for (int curC = 0; curC < 3; curC++) {
            eMatrix[curR][curC] = camCurrent.rotation[curR][curC];
          }
        }
        eMatrix[0][3] = camCurrent.translation[0];
        eMatrix[1][3] = camCurrent.translation[1];
        eMatrix[2][3] = camCurrent.translation[2];
        eMatrix[3][3] = 1.0;

        for (int curR = 0; curR < 3; curR++) {
          for (int curC = 0; curC < 3; curC++) {
            kMatrix[curR][curC] = sfm_data_.camerasList_[curCam].intrinsics[curR][curC];
          }
        }


        sfm_data_.camerasList_[curCam].cameraMatrix =  eMatrix*kMatrix;
        sfm_data_.camerasList_[curCam].rotation = glm::mat3(camCurrent.rotation);
        sfm_data_.camerasList_[curCam].translation = glm::vec3(camCurrent.translation);
        sfm_data_.camerasList_[curCam].center = glm::vec3(camCurrent.center);
        sfm_data_.camerasList_[curCam].valid = true;
       /* std::cout<<"curCam: "<<curCam<<std::endl;
        std::cout<<"Intrinsics A"<<std::endl;
        utilities::printMatrix(kMatrix);
        std::cout<<"Intrinsics B"<<std::endl;
        utilities::printMatrix(sfm_data_.camerasList_[curCam].intrinsics);
        std::cout<<"Rotation"<<std::endl;
        utilities::printMatrix(sfm_data_.camerasList_[curCam].rotation);
        std::cout<<"traslation"<<std::endl;
        utilities::printMatrix(sfm_data_.camerasList_[curCam].translation);
        std::cout<<"Extrinsics"<<std::endl;
        utilities::printMatrix(eMatrix);
        std::cout<<"camera matrix"<<std::endl;
        utilities::printMatrix(sfm_data_.camerasList_[curCam].cameraMatrix);*/

      } catch (std::out_of_range e) {
        sfm_data_.camerasList_[curCam].valid = false;
        std::cerr << "std::out_of_range exception trying to look for extrinsics matrix " << idExtrinsics << std::endl;
      }
    }
  } catch (JsonAccessException& e) {
    std::cerr << e.what() << std::endl;
    std::cout << e.what() << std::endl;

  }

}


void OpenMvgParser::parseIntrinsics(std::map<int, glm::mat3> & intrinsics,std::map<int, glm::vec3> &distortion) {

  try {

    if (!document_.HasMember("intrinsics"))
      throw JsonAccessException("JsonAccessException--> error while querying HasMember(views)");
    const rapidjson::Value& intrinsicsJson = document_["intrinsics"];

    for (rapidjson::SizeType curInt = 0; curInt < intrinsicsJson.Size(); curInt++) {
      int key = intrinsicsJson[curInt]["key"].GetInt();
      glm::mat3 temp(0.0);
      temp[0][0] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["focal_length"].GetFloat();
      temp[1][1] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["focal_length"].GetFloat();
      temp[0][2] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["principal_point"][0].GetFloat();
      temp[1][2] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["principal_point"][1].GetFloat();
      temp[2][2] = 1.0;
      glm::vec3 tempdist;
      if (document_.HasMember("disto_k3"))
        tempdist = glm::vec3(temp[0][2] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["disto_k3"][0].GetFloat(),
            temp[0][2] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["disto_k3"][1].GetFloat(),
            temp[0][2] = intrinsicsJson[curInt]["value"]["ptr_wrapper"]["data"]["disto_k3"][2].GetFloat());

      intrinsics.insert(std::pair<int, glm::mat3>(key, temp));
      distortion.insert(std::pair<int, glm::vec3>(key, tempdist));
    }

  } catch (JsonAccessException& e) {
    std::cerr << e.what() << std::endl;
    std::cout << e.what() << std::endl;

  }


}

void OpenMvgParser::parseExtrinsics(std::map<int, CameraType> & extrinsics) {

  try {

    if (!document_.HasMember("extrinsics"))
      throw JsonAccessException("JsonAccessException--> error while querying HasMember(extrinsics)");
    const rapidjson::Value& extrinsicsJson = document_["extrinsics"];

    for (rapidjson::SizeType curInt = 0; curInt < extrinsicsJson.Size(); curInt++) {
      int key = extrinsicsJson[curInt]["key"].GetInt();
      CameraType temp;

      for (int curR = 0; curR < 3; curR++) {
        for (int curC = 0; curC < 3; curC++) {
          temp.rotation[curR][curC] = extrinsicsJson[curInt]["value"]["rotation"][curR][curC].GetFloat();
        }
      }

      for (int curR = 0; curR < 3; curR++) {
        temp.center[curR] = extrinsicsJson[curInt]["value"]["center"][curR].GetFloat();
      }
      temp.translation = -temp.center * temp.rotation;

      extrinsics.insert(std::pair<int, CameraType>(key, temp));
    }

  } catch (JsonAccessException& e) {
    std::cerr << e.what() << std::endl;
    std::cout << e.what() << std::endl;

  }
}

OpenMvgParser::OpenMvgParser() {
}

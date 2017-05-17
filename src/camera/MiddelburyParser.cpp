#include <MiddelburyParser.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <utilities.hpp>

MiddelburyParser::MiddelburyParser(std::string fileName) {
  fileStream_.open(fileName.c_str(),std::ios::in);
}

MiddelburyParser::~MiddelburyParser() {
}

bool MiddelburyParser::parse() {

  std::string line;

  //read num cam
  std::getline(fileStream_, line);
  std::istringstream iss(line);
  iss >> numCameras_;

  for (int curCam = 0; curCam < numCameras_; curCam = curCam + 1) {
    CameraType tempCamera;

    //discard the line of cams to be skipped
    for (int var = 0; var < 1; ++var) {
      std::getline(fileStream_, line);
    }

    iss.str(line);

    char path[1000];
    //std::cout<<"tempCamera.cameraMatrix "<<iss.str()<<std::endl;
    sscanf(iss.str().c_str(),"%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ", path,
        &tempCamera.intrinsics[0][0] , &tempCamera.intrinsics[0][1] , &tempCamera.intrinsics[0][2] ,
        &tempCamera.intrinsics[1][0] , &tempCamera.intrinsics[1][1] , &tempCamera.intrinsics[1][2] ,
        &tempCamera.intrinsics[2][0] , &tempCamera.intrinsics[2][1] , &tempCamera.intrinsics[2][2] ,
        &tempCamera.rotation[0][0] , &tempCamera.rotation[0][1] , &tempCamera.rotation[0][2] ,
        &tempCamera.rotation[1][0] , &tempCamera.rotation[1][1] , &tempCamera.rotation[1][2] ,
        &tempCamera.rotation[2][0] , &tempCamera.rotation[2][1] , &tempCamera.rotation[2][2] ,
        &tempCamera.translation[0] , &tempCamera.translation[1] , &tempCamera.translation[2]);

    tempCamera.cameraPath = std::string(path);



    glm::mat4 tempCameraExtrinsic(0.0);
    tempCameraExtrinsic[0][0] = tempCamera.rotation[0][0];
    tempCameraExtrinsic[0][1] = tempCamera.rotation[0][1];
    tempCameraExtrinsic[0][2] = tempCamera.rotation[0][2];
    tempCameraExtrinsic[1][0] = tempCamera.rotation[1][0];
    tempCameraExtrinsic[1][1] = tempCamera.rotation[1][1];
    tempCameraExtrinsic[1][2] = tempCamera.rotation[1][2];
    tempCameraExtrinsic[2][0] = tempCamera.rotation[2][0];
    tempCameraExtrinsic[2][1] = tempCamera.rotation[2][1];
    tempCameraExtrinsic[2][2] = tempCamera.rotation[2][2];


    tempCameraExtrinsic[0][3] = tempCamera.translation[0];
    tempCameraExtrinsic[1][3] = tempCamera.translation[1];
    tempCameraExtrinsic[2][3] = tempCamera.translation[2];

    glm::mat4 tempCameraIntrinsicH(0.0);
    tempCameraIntrinsicH[0][0] = tempCamera.intrinsics[0][0];
    tempCameraIntrinsicH[0][1] = tempCamera.intrinsics[0][1];
    tempCameraIntrinsicH[0][2] = tempCamera.intrinsics[0][2];
    tempCameraIntrinsicH[1][0] = tempCamera.intrinsics[1][0];
    tempCameraIntrinsicH[1][1] = tempCamera.intrinsics[1][1];
    tempCameraIntrinsicH[1][2] = tempCamera.intrinsics[1][2];
    tempCameraIntrinsicH[2][0] = tempCamera.intrinsics[2][0];
    tempCameraIntrinsicH[2][1] = tempCamera.intrinsics[2][1];
    tempCameraIntrinsicH[2][2] = tempCamera.intrinsics[2][2];
    tempCamera.cameraMatrix = glm::transpose(glm::transpose(tempCameraIntrinsicH) * glm::transpose(tempCameraExtrinsic));
   
    tempCamera.center = - tempCamera.translation * glm::transpose(tempCamera.rotation);

    camerasList_.push_back(tempCamera);
  }

  sfm_data_.camerasList_ = camerasList_;
  for (int curCam = 0; curCam < sfm_data_.camerasList_.size(); curCam++) {
    utilities::convertToMvp2(sfm_data_.camerasList_[curCam],sfm_data_.camerasList_[curCam].mvp);
    sfm_data_.camerasList_[curCam].imageWidth  = cv::imread(sfm_data_.camerasList_[curCam].cameraPath).cols;
    sfm_data_.camerasList_[curCam].imageHeight = cv::imread(sfm_data_.camerasList_[curCam].cameraPath).rows;
  }
  sfm_data_.imageWidth_  = sfm_data_.camerasList_[0].imageWidth;
  sfm_data_.imageHeight_ = sfm_data_.camerasList_[0].imageHeight;
  sfm_data_.numCameras_  = sfm_data_.camerasList_.size();


  return true;
}

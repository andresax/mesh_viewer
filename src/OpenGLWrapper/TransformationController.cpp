
#include "TransformationController.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <gtc/matrix_inverse.hpp>
#include <iostream>

#include "utilities.hpp"

TransformationController::TransformationController(GLFWwindow* window, float imageWidth, float imageHeight) {
  window_ = window;

  //initial ViewMatrix
  viewMatrix_ = glm::mat4();
  modelMatrix_ = glm::mat4();
  projectionMatrix_ = glm::mat4();
  imageWidth_ = imageWidth;
  imageHeight_ = imageHeight;
  curCam_ = 0;
  curRottemp_ = glm::mat4(1.0);

}

TransformationController::TransformationController(const float imageWidth, const float imageHeight) {
  window_ = nullptr;

  //initial ViewMatrix
  viewMatrix_ = glm::mat4();
  modelMatrix_ = glm::mat4();
  projectionMatrix_ = glm::mat4();
  imageWidth_ = imageWidth;
  imageHeight_ = imageHeight;
  curRottemp_ = glm::mat4(1.0);
  curCam_ = 0;

//  imageWidth_ = 316.7*2;
//  imageHeight_ = 200.6*2;
}
TransformationController::~TransformationController() {

}

glm::mat4 TransformationController::getProjectionMatrix() {
  return projectionMatrix_;
}

glm::mat4 TransformationController::getViewMatrix() {
  return viewMatrix_;
}

glm::mat4 TransformationController::getMvpMatrix() {
  return mvp_;
}

glm::mat4 TransformationController::getCurMatrix() {
  // glm::mat4 mvp = projectionMatrix_ * viewMatrix_ * modelMatrix_;

  return mvp_;
}

void TransformationController::setRotation(const glm::mat3& rotation) {
  rotation_ = rotation;

  recomputeViewMatrix();
}

void TransformationController::setTranslation(const glm::vec3& translation) {
  translation_ = translation;

  recomputeViewMatrix();
}

void TransformationController::setIntrinsicParameters(float fx, float fy, float cx, float cy) {

  float N = 0.10;
  float F = 10000.0;

  // Pay attention this matrix is the transpose of the actual projection matrix
  //  since openGL uses the column-major convention
  /*projectionMatrix_ = glm::mat4(0);
  projectionMatrix_[0][0] = fx / cx;
  projectionMatrix_[1][1] = -fy / cy;
  projectionMatrix_[2][2] = (N + F) / (F - N);
  projectionMatrix_[3][2] = -(2 * N * F) / (F - N);
  projectionMatrix_[2][3] = 1.0;*/
  /*std::cout <<"setIntrinsicParameters"<<std::endl;
   utils::printMatrix(projectionMatrix_);*/

  glm::mat4 persp = glm::mat4(0.0);
  persp[0][0] = fx;
  persp[0][1] = 0.0;
  persp[0][2] = cx;
  persp[1][1] = fy;
  persp[1][2] = cy;
  persp[2][2] = -(N + F);
  persp[2][3] = N * F;
  persp[3][2] = 1.0;

  double L = 0;
  double R = imageWidth_;
  double B = 0;
  double T = imageHeight_;
  glm::mat4 ortho = glm::mat4(0.0);

  int dino = 1;/*to change the y sign if dino is used*/
  ortho[0][0] = 2.0 / (R - L);
  ortho[0][3] = -(R + L) / (R - L);
  ortho[1][1] = (1-2*dino)*2.0 / (T - B);
  ortho[1][3] = -(1-2*dino)*(T + B) / (T - B);
  ortho[2][2] = (-2.0 / (F - N));
  ortho[2][3] = (-(F + N) / (F - N));
  ortho[3][3] = 1.0;
  projectionMatrix_ = glm::transpose(persp * ortho);
  /*std::cout <<"projectionMatrix_"<<std::endl;
   utils::printMatrix(projectionMatrix_);
   std::cout <<"persp"<<std::endl;
    utils::printMatrix(persp);
    std::cout <<"ortho"<<std::endl;
     utils::printMatrix(ortho);*/
}

void TransformationController::setExtrinsicParameters(const glm::mat3& rotation, const glm::vec3& translation) {
  rotation_ = rotation;
  translation_ = translation;

  recomputeViewMatrix();
}

void TransformationController::computeMatricesFromCam() {
  recomputeViewMatrix();
}

glm::vec3 TransformationController::getCameraCenter() {
  glm::vec3 temp =  - translation_ * glm::transpose(rotation_);
  //utils::printMatrix(temp);
  return temp;

}

glm::mat4 TransformationController::getInitMvpMatrix() {
  return initmvp_;
}

void TransformationController::recomputeViewMatrix() {

  glm::mat3 rotTmp = rotation_;

  glm::vec3 trTmp = translation_;

  // Pay attention this matrix is the transpose of the actual view matrix
  //  since openGL uses the column-major convention
  viewMatrix_ = glm::mat4();
  viewMatrix_[0][0] = rotTmp[0][0];
  viewMatrix_[0][1] = rotTmp[1][0];
  viewMatrix_[0][2] = rotTmp[2][0];
  viewMatrix_[1][0] = rotTmp[0][1];
  viewMatrix_[1][1] = rotTmp[1][1];
  viewMatrix_[1][2] = rotTmp[2][1];
  viewMatrix_[2][0] = rotTmp[0][2];
  viewMatrix_[2][1] = rotTmp[1][2];
  viewMatrix_[2][2] = rotTmp[2][2];
/*
  viewMatrix_[0][0] = rotTmp[0][0];
  viewMatrix_[0][1] = rotTmp[0][1];
  viewMatrix_[0][2] = rotTmp[0][2];
  viewMatrix_[1][0] = rotTmp[1][0];
  viewMatrix_[1][1] = rotTmp[1][1];
  viewMatrix_[1][2] = rotTmp[2][2];
  viewMatrix_[2][0] = rotTmp[2][0];
  viewMatrix_[2][1] = rotTmp[2][1];
  viewMatrix_[2][2] = rotTmp[2][2];*/
  viewMatrix_[3][0] = trTmp.x;
  viewMatrix_[3][1] = trTmp.y;
  viewMatrix_[3][2] = trTmp.z;
  viewMatrix_[0][3] = 0.0;
  viewMatrix_[1][3] = 0.0;
  viewMatrix_[2][3] = 0.0;
  viewMatrix_[3][3] = 1.0;

 /* std::cout <<"viewMatrix_"<<std::endl;
  utils::printMatrix(viewMatrix_);*/
  mvp_ = projectionMatrix_* curRottemp_ *viewMatrix_ * modelMatrix_;
  mvp_ = projectionMatrix_ * viewMatrix_ * modelMatrix_;
  /*std::cout <<"mvp_"<<std::endl;
  utils::printMatrix(mvp_);*/

}

void TransformationController::moveCamFromInputs() {

  if (window_ == nullptr) {
    std::cout << "TransformationController::moveCamFromInputs: you need to set the window context "
        "if you want to use user interaction" << std::endl;
  } else {
    float deltatr = 0.05;
    float deltaTetha = 0.001;
    glm::vec3 trTmp = glm::vec3(0);
    glm::mat3 rotTmp = glm::mat3();
    bool updateMatrix = false;
    // Move forward
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
      trTmp = glm::vec3(deltatr, 0, 0);
      updateMatrix = true;
    }
    // Move backward
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
      trTmp = glm::vec3(-deltatr, 0, 0);
      updateMatrix = true;
    }
    // Strafe right
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
      trTmp = glm::vec3(0, deltatr, 0);
      updateMatrix = true;
    }
    // Strafe left
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
      trTmp = glm::vec3(0, -deltatr, 0);
      updateMatrix = true;
    }
    // Go DOWN
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
      trTmp = glm::vec3(0, 0, deltatr);
      updateMatrix = true;
    }
    // Go UP
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
      trTmp = glm::vec3(0, 0, -deltatr);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS) {
      rotTmp = utils::rotX(deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_O) == GLFW_PRESS) {
      rotTmp = utils::rotX(-deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS) {
      rotTmp = utils::rotY(deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_K) == GLFW_PRESS) {
      rotTmp = utils::rotY(-deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_M) == GLFW_PRESS) {
      rotTmp = utils::rotZ(deltaTetha);
      updateMatrix = true;
    }


    if (glfwGetKey(window_, GLFW_KEY_N) == GLFW_PRESS) {
      rotTmp = utils::rotZ(-deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_X) == GLFW_PRESS) {
      CameraType cam = camvec_[curCam_];
      setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
      setExtrinsicParameters(cam.rotation, cam.translation);

      ++curCam_;

    }
    if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
      CameraType cam = camvec_[curCam_];
      setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
      setExtrinsicParameters(cam.rotation, cam.translation);
      curCam_ += 5;
    }
    if (glfwGetKey(window_, GLFW_KEY_V) == GLFW_PRESS) {
      CameraType cam = camvec_[curCam_];
      setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
      setExtrinsicParameters(cam.rotation, cam.translation);
      curCam_ += 10;
    }
    if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
      CameraType cam = camvec_[curCam_];
      setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
      setExtrinsicParameters(cam.rotation, cam.translation);
      curCam_--;
    }

    if (updateMatrix == true) {
      glm::mat4 tm;

      tm[0][0] = rotTmp[0][0];
      tm[0][1] = rotTmp[1][0];
      tm[0][2] = rotTmp[2][0];
      tm[1][0] = rotTmp[0][1];
      tm[1][1] = rotTmp[1][1];
      tm[1][2] = rotTmp[2][1];
      tm[2][0] = rotTmp[0][2];
      tm[2][1] = rotTmp[1][2];
      tm[2][2] = rotTmp[2][2];

      tm[3][0] = trTmp[0];
      tm[3][1] = trTmp[1];
      tm[3][2] = trTmp[2];
      tm[0][3] = 0.0;
      tm[1][3] = 0.0;
      tm[2][3] = 0.0;
      tm[3][3] = 1.0;
      curRottemp_ = tm * curRottemp_;
      recomputeViewMatrix();
      //std::cout<<"mmm"<<std::endl;
      //utils::printMatrix(curRottemp_*viewMatrix_);
    }
  }
}


void TransformationController::moveModelFromInputs() {

  if (window_ == nullptr) {
    std::cout << "TransformationController::moveCamFromInputs: you need to set the window context "
        "if you want to use user interaction" << std::endl;
  } else {
    float deltatr = 0.05;
    float deltaTetha = 0.001;
    glm::vec3 trTmp = glm::vec3(0);
    glm::mat3 rotTmp = glm::mat3();
    bool updateMatrix = false;
    // Move forward
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
      trTmp = glm::vec3(deltatr, 0, 0);
      updateMatrix = true;
    }
    // Move backward
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
      trTmp = glm::vec3(-deltatr, 0, 0);
      updateMatrix = true;
    }
    // Strafe right
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
      trTmp = glm::vec3(0, deltatr, 0);
      updateMatrix = true;
    }
    // Strafe left
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
      trTmp = glm::vec3(0, -deltatr, 0);
      updateMatrix = true;
    }
    // Go DOWN
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
      trTmp = glm::vec3(0, 0, deltatr);
      updateMatrix = true;
    }
    // Go UP
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
      trTmp = glm::vec3(0, 0, -deltatr);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS) {
      rotTmp = utils::rotX(deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_O) == GLFW_PRESS) {
      rotTmp = utils::rotX(-deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS) {
      rotTmp = utils::rotY(deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_K) == GLFW_PRESS) {
      rotTmp = utils::rotY(-deltaTetha);
      updateMatrix = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_M) == GLFW_PRESS) {
      rotTmp = utils::rotZ(deltaTetha);
      updateMatrix = true;
    }


    if (glfwGetKey(window_, GLFW_KEY_N) == GLFW_PRESS) {
      rotTmp = utils::rotZ(-deltaTetha);
      updateMatrix = true;
    }



    if (updateMatrix == true) {
      glm::mat4 tm;

      tm[0][0] = rotTmp[0][0];
      tm[0][1] = rotTmp[1][0];
      tm[0][2] = rotTmp[2][0];
      tm[1][0] = rotTmp[0][1];
      tm[1][1] = rotTmp[1][1];
      tm[1][2] = rotTmp[2][1];
      tm[2][0] = rotTmp[0][2];
      tm[2][1] = rotTmp[1][2];
      tm[2][2] = rotTmp[2][2];

      tm[3][0] = trTmp[0];
      tm[3][1] = trTmp[1];
      tm[3][2] = trTmp[2];
      tm[0][3] = 0.0;
      tm[1][3] = 0.0;
      tm[2][3] = 0.0;
      tm[3][3] = 1.0;
      curRottemp_ = tm * curRottemp_;
      mvp_ = projectionMatrix_  *viewMatrix_ * curRottemp_ ;
    }
  }
}

void TransformationController::computeMatricesFromCam(int camIdx) {
  CameraType cam = camvec_[camIdx];
  setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
  setExtrinsicParameters(cam.rotation, cam.translation);
}

void TransformationController::fixInitCam() {

  initmvp_ = mvp_;
}

/*
 * TransformationController.h
 *
 *  Created on: 16/giu/2014
 *      Author: andrea
 */

#ifndef TRANSFORMATIONCONTROLLER_H_
#define TRANSFORMATIONCONTROLLER_H_

#include <glm.hpp>
#include <GLFW/glfw3.h>
#include "types.hpp"

class TransformationController {
  public:
    TransformationController(GLFWwindow* window, const float imageWidth, const float imageHeight);
    TransformationController(const float imageWidth, const float imageHeight);
    virtual ~TransformationController();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::mat4 getMvpMatrix();
    glm::mat4 getInitMvpMatrix();
    glm::mat4 getCurMatrix();
    glm::vec3 getCameraCenter();

    void setRotation(const glm::mat3& rotation);
    void setTranslation(const glm::vec3& translation);
    void setIntrinsicParameters(float fx, float fy, float cx, float cy);
    void setExtrinsicParameters(const glm::mat3& rotation,const  glm::vec3& translation);

    void moveCamFromInputs();
    void moveModelFromInputs();
    void computeMatricesFromCam();

    void computeMatricesFromCam(int camIdx);
    void fixInitCam();

    void recomputeViewMatrix();

    int getImageHeight() const {
      return imageHeight_;
    }

    void setImageHeight(int imageHeight) {
      imageHeight_ = imageHeight;
    }
    void setCamList(std::vector<CameraType> camvec){
        camvec_ = camvec;
    }

    int getImageWidth() const {
      return imageWidth_;
    }

    void setImageWidth(int imageWidth) {
      imageWidth_ = imageWidth;
    }

    void setWindow(GLFWwindow*& window) {
      window_ = window;
    }

    int getCurCam() const {
      return curCam_;
    }

    void setCurCam(int curCam) {
      curCam_ = curCam;
    }

  const glm::mat3& getRotation() const {
    return rotation_;
  }

  const glm::vec3& getTranslation() const {
    return translation_;
  }

  private:
    GLFWwindow* window_;

    glm::mat3 rotation_;
    glm::mat4 curRottemp_;
    glm::vec3 translation_;

    glm::mat4 mvp_;
    glm::mat4 initmvp_;
    glm::mat4 curmvp_;

    glm::mat4 projectionMatrix_;
    glm::mat4 viewMatrix_;
    glm::mat4 modelMatrix_;
    std::vector<CameraType> camvec_;
    int curCam_;

    int imageWidth_, imageHeight_;

    //void mouseWheelCallback(GLFWwindow* window, double x, double y);
};

#endif /* TRANSFORMATIONCONTROLLER_H_ */

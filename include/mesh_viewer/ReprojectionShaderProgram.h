/*
 * ReprojectionShaderProgram.h
 *
 *  Created on: 03/apr/2015
 *      Author: andrea
 */

#ifndef SWEEPMESH_REPROJECTIONSHADERPROGRAM_H_
#define SWEEPMESH_REPROJECTIONSHADERPROGRAM_H_

#include "ShaderProgram.h"
#include <glm.hpp>

class ReprojectionShaderProgram : public ShaderProgram {
public:
  ReprojectionShaderProgram(int imageWidth, int imageHeight);
  virtual ~ReprojectionShaderProgram();
  void initTex();
  void populateTex(const cv::Mat &image);

  void initializeFramebufAndTex(GLuint &imageReprojTex);

  void compute(bool renderFrameBuf = false);

  void setDepthTexture(GLuint depthTexture) {
    depthTexture_ = depthTexture;
  }

  void setMvp(const glm::mat4& mvp) {
    mvp_ = mvp;
  }

  void setCamCenter(const glm::vec3& camCenter) {
    camCenter_ = camCenter;
  }

  void setVertexArrayObj(GLuint vertexArrayObj) {
    vertexArrayObj_ = vertexArrayObj;
  }
  void setVertexArrayGradObj(GLuint arrayPlusBufferObj) {
    arrayPlusBufferObj_ = arrayPlusBufferObj;
  }
private:
  void init();

  void createAttributes();
  void createUniforms();

  GLuint framebufferReproj_;
  GLuint imageReprojTex_;
  GLuint imageTex_;

  GLuint posAttribReprojId_, mvpIDReproj_, imageTexId_, shadowMapIdReproj_,vertexArrayObj_,arrayPlusBufferObj_,normalAttribsimGradId_;

  GLuint depthTexture_, camCenterID_;
  glm::vec3 camCenter_;

  glm::mat4 mvp_;

};

#endif /* SWEEPMESH_REPROJECTIONSHADERPROGRAM_H_ */

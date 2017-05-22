/*
 * ReprojectionShaderProgram.cpp
 *
 *  Created on: 03/apr/2015
 *      Author: andrea
 */

#include <ReprojectionShaderProgram.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <utilities.hpp>

ReprojectionShaderProgram::ReprojectionShaderProgram(int imageWidth, int imageHeight) :
    ShaderProgram(imageWidth, imageHeight) {
  framebufferReproj_ = imageReprojTex_ = imageTex_ = -1;
  posAttribReprojId_ = mvpIDReproj_  = imageTexId_ = shadowMapIdReproj_ = -1;
  depthTexture_ = -1;
  camCenterID_ = -1;
}

ReprojectionShaderProgram::~ReprojectionShaderProgram() {
}

void ReprojectionShaderProgram::initTex() {
  glGenTextures(1, &imageTex_);
  glBindTexture(GL_TEXTURE_2D, imageTex_);
  defaultTextureParameters();
}

void ReprojectionShaderProgram::populateTex(const cv::Mat& image) {

  cv::Mat image2Gray;
  cv::cvtColor(image, image2Gray, CV_RGB2GRAY);
  glBindTexture(GL_TEXTURE_2D, imageTex_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageWidth_, imageHeight_, 0, GL_RED, GL_UNSIGNED_BYTE, image2Gray.data);

  image2Gray.release();
}

void ReprojectionShaderProgram::compute(bool renderFrameBuf) {
  if (renderFrameBuf) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferReproj_);
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
  glDrawBuffers(2, attachments);

  glViewport(0, 0, imageWidth_, imageHeight_);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  shaderManager_.enable();

  glUniformMatrix4fv(mvpIDReproj_, 1, GL_FALSE, &mvp_[0][0]);
  glUniform3fv(camCenterID_, 1,&camCenter_[0]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depthTexture_);
  glUniform1i(shadowMapIdReproj_, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, imageTex_);
  glUniform1i(imageTexId_, 1);

  glBindVertexArray(vertexArrayObj_);
  glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj_);
  glVertexAttribPointer(posAttribReprojId_, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, arrayPlusBufferObj_);
  glVertexAttribPointer(normalAttribsimGradId_, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glEnableVertexAttribArray(posAttribReprojId_);
  glEnableVertexAttribArray(normalAttribsimGradId_);

  glBindVertexArray(vertexArrayObj_);
  glDrawArrays(GL_TRIANGLES, 0, sizeArray_);

  glDisableVertexAttribArray(normalAttribsimGradId_);

  glDisableVertexAttribArray(posAttribReprojId_);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReprojectionShaderProgram::initializeFramebufAndTex(GLuint &imageReprojTex) {
  glGenFramebuffers(1, &framebufferReproj_);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferReproj_);
  initRedTex(imageReprojTex);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, imageReprojTex, 0);
  GLuint tmpdeptht;
  initDepthTex(tmpdeptht);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tmpdeptht, 0);
  checkFrameBuffer("ReprojectionShaderProgram::initializeFramebufAndTex");
  imageReprojTex_ = imageReprojTex;
}

void ReprojectionShaderProgram::init() {
  shaderManager_.init();
  shaderManager_.addShader(GL_VERTEX_SHADER, "shaders/reprojection_vertex_shader.glsl");
  shaderManager_.addShader(GL_GEOMETRY_SHADER, "shaders/reprojection_geometry_shader.glsl");
  shaderManager_.addShader(GL_FRAGMENT_SHADER, "shaders/reprojection_fragment_shader.glsl");
  shaderManager_.finalize();
}

void ReprojectionShaderProgram::createAttributes() {
  posAttribReprojId_ = shaderManager_.getAttribLocation("position");
  normalAttribsimGradId_ = shaderManager_.getAttribLocation("infoSurface");
}

void ReprojectionShaderProgram::createUniforms() {
  mvpIDReproj_ = shaderManager_.getUniformLocation("MVP");
  imageTexId_ = shaderManager_.getUniformLocation("image");
  camCenterID_ = shaderManager_.getUniformLocation("camCenter");
  shadowMapIdReproj_ = shaderManager_.getUniformLocation("shadowMap1");
}


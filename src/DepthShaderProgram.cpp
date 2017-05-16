/*
 * DepthShaderProgram.cpp
 *
 *  Created on: 02/apr/2015
 *      Author: andrea
 */

#include "DepthShaderProgram.h"

#include <glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

DepthShaderProgram::DepthShaderProgram(int imageWidth, int imageHeight) :
    ShaderProgram(imageWidth, imageHeight) {
  posAttribDepthId_ = -1;
  mvpId_  = -1;
}

DepthShaderProgram::~DepthShaderProgram() {
}

void DepthShaderProgram::initializeFramebufAndTex(GLuint& framebufferDepth, GLuint& depthTextureId) {
  glGenFramebuffers(1, &framebufferDepth);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferDepth);
  initDepthTex(depthTextureId);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureId, 0);
  glDrawBuffer(GL_NONE);
  checkFrameBuffer("DepthShaderProgram::initializeFramebufAndTex");
}

void DepthShaderProgram::computeDepthMap(const GLuint &framebufferDepth, const glm::mat4 &mvp) {
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferDepth);
  glViewport(0, 0, imageWidth_, imageHeight_);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shaderManager_.enable();

  glEnableVertexAttribArray(posAttribDepthId_);
  glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj_);
  glVertexAttribPointer(posAttribDepthId_, 3, GL_FLOAT, GL_FALSE, 0, 0);


  if (useElements_Indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferObj_);
    glDrawElements(GL_TRIANGLES, numElements_, GL_UNSIGNED_INT, 0);

  } else {
    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj_);
    glDrawArrays(GL_TRIANGLES, 0, sizeArray_);

  }

  glDisableVertexAttribArray(posAttribDepthId_);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthShaderProgram::init() {
  shaderManager_.init();
  shaderManager_.addShader(GL_VERTEX_SHADER, "shaders/depth_vertex_shader.glsl");
  shaderManager_.addShader(GL_FRAGMENT_SHADER, "shaders/depth_fragment_shader.glsl");
  shaderManager_.finalize();
}

void DepthShaderProgram::createAttributes() {

  posAttribDepthId_ = shaderManager_.getAttribLocation("position");
}
void DepthShaderProgram::createUniforms() {
  mvpId_ = shaderManager_.getUniformLocation("MVP");

}
void DepthShaderProgram::createTransformFeedback() {

}


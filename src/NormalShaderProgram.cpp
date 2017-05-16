/*
 * NormalShaderProgram.cpp
 *
 *  Created on: 04/apr/2015
 *      Author: andrea
 */

#include "NormalShaderProgram.h"

NormalShaderProgram::NormalShaderProgram(int imageWidth, int imageHeight) :
    ShaderProgram(imageWidth, imageHeight) {
  posAttrib_ = -1;
  feedbackBuffer_ = feedbackLength_ = -1;

}

NormalShaderProgram::~NormalShaderProgram() {
}

void NormalShaderProgram::compute() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  shaderManager_.enable();

  glEnableVertexAttribArray(posAttrib_);
  glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj_);
  glVertexAttribPointer(posAttrib_, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glEnable(GL_RASTERIZER_DISCARD);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayBufferObj_);

  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackBuffer_);/*addd*/
  glBeginTransformFeedback(GL_TRIANGLES);

  if (useElements_Indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBufferObj_);
    glDrawElements(GL_TRIANGLES, numElements_, GL_UNSIGNED_INT, 0);

  } else {

    glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj_);
    glDrawArrays(GL_TRIANGLES, 0, sizeArray_);

  }
  glEndTransformFeedback();
  glDisableVertexAttribArray(posAttrib_);

  glFinish();
  glDisable(GL_RASTERIZER_DISCARD);

  if (useElements_Indices) {
    feedbackTr.resize(numElements_, glm::vec3(0.0));
    feedbackLength_ = numElements_;

  } else {
    feedbackTr.resize(sizeArray_, glm::vec3(0.0));
    feedbackLength_ = sizeArray_;

  }
  glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackLength_ * sizeof(glm::vec3), &feedbackTr[0]);
   /* for (auto f : feedbackTr) {
      std::cout << "F x: " << f.x << ", y: " << f.y << ", z: " << f.z << std::endl;
    }*/
}

void NormalShaderProgram::init() {

  shaderManager_.init();
  shaderManager_.addShader(GL_VERTEX_SHADER, "shaders/normal_vertex_shader.glsl");
  shaderManager_.addShader(GL_GEOMETRY_SHADER, "shaders/normal_geometry_shader.glsl");
  shaderManager_.addFeedbackTransform("normal");
  shaderManager_.finalize();
}

void NormalShaderProgram::createAttributes() {
  posAttrib_ = shaderManager_.getAttribLocation("position");
}

void NormalShaderProgram::createUniforms() {
}

void NormalShaderProgram::createTransformFeedback(int length) {
  glGenBuffers(1, &feedbackBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER, feedbackBuffer_);
  glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec3), nullptr, GL_DYNAMIC_READ);

}

void NormalShaderProgram::resetTransformFeedback(int length) {
  glBindBuffer(GL_ARRAY_BUFFER, feedbackBuffer_);
  glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec3), nullptr, GL_DYNAMIC_READ);

}

/*
 * NormalShaderProgram.h
 *
 *  Created on: 04/apr/2015
 *      Author: andrea
 */

#ifndef SWEEPMESH_NORMALSHADERPROGRAM_H_
#define SWEEPMESH_NORMALSHADERPROGRAM_H_

#include "ShaderProgram.h"
#include <glm.hpp>

class NormalShaderProgram : public ShaderProgram {
public:
  NormalShaderProgram(int imageWidth, int imageHeight);
  virtual ~NormalShaderProgram();
  void compute();
  void createTransformFeedback(int length);
  void resetTransformFeedback(int length);

  void setFeedbackLength(GLuint feedbackLength) {
    feedbackLength_ = feedbackLength;
  }

  const std::vector<glm::vec3>& getFeedbackTr() const {
    return feedbackTr;
  }

private:
  void init();

  void createAttributes();
  void createUniforms();

  GLuint posAttrib_;

  GLuint feedbackBuffer_;
  GLuint feedbackLength_;
  std::vector<glm::vec3> feedbackTr;
};

#endif /* SWEEPMESH_NORMALSHADERPROGRAM_H_ */

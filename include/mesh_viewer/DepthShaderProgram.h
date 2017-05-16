/*
 * DepthShaderProgram.h
 *
 *  Created on: 02/apr/2015
 *      Author: andrea
 */

#ifndef DEPTHSHADERPROGRAM_H_
#define DEPTHSHADERPROGRAM_H_

#include "ShaderProgram.h"
#include <glm.hpp>

class DepthShaderProgram : public ShaderProgram{
public:
  DepthShaderProgram(int imageWidth, int imageHeight);
  virtual ~DepthShaderProgram();

  void initTexAndFramebuf();

  void initializeFramebufAndTex(GLuint& framebufferDepth, GLuint& depthTextureId);
  void computeDepthMap(const GLuint &framebufferDepth, const glm::mat4 &mvp);

  void setMvp(const glm::mat4& mvp) {
    mvp_ = mvp;
  }

private:
  void init();

  void createAttributes();
  void createUniforms();
  void createTransformFeedback();

  GLuint posAttribDepthId_;
  GLuint mvpId_ ;

  glm::mat4 mvp_;

};

#endif /* DEPTHSHADERPROGRAM_H_ */

#ifndef OPENGLWRAPPER_OPENGLPROGRAM_H_
#define OPENGLWRAPPER_OPENGLPROGRAM_H_

#include <GL/glew.h>
#include <opencv2/core/core.hpp>

struct GLFWwindow;

class OpenGLProgram {
public:
  OpenGLProgram();
  virtual ~OpenGLProgram();

  virtual void run() = 0;

  void setImageHeight(int imageHeight) {
    imageHeight_ = imageHeight;
  }

  void setImageWidth(int imageWidth) {
    imageWidth_ = imageWidth;
  }

protected:

  void SwapBuffers();
  void CaptureViewPort(cv::Mat &capture, GLenum format = GL_BGR_EXT,  GLuint ch=3);
  void CaptureViewPortFloat(cv::Mat &capture, GLenum format = GL_BGR_EXT, GLuint ch=3);
  bool initGlwf();
  void init();
  void checkLastError();

  GLFWwindow* window_;
  int imageWidth_, imageHeight_;
  GLuint vertexArrayObj_;
};

#endif /* OPENGLWRAPPER_OPENGLPROGRAM_H_ */

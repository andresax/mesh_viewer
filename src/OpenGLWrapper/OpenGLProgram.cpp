/*
 * OpenGLProgram.cpp
 *
 *  Created on: 01/apr/2015
 *      Author: andrea
 */

#include "OpenGLProgram.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdio>
#include <iostream>

OpenGLProgram::OpenGLProgram() {
  window_ = nullptr;
  imageWidth_ = -1;
  imageHeight_ = -1;
  vertexArrayObj_ = -1;
}

OpenGLProgram::~OpenGLProgram() {

}

void OpenGLProgram::CaptureViewPort(cv::Mat &capture, GLenum format, GLuint ch) {

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLubyte * bits; //RGB bits
  GLint viewport[4]; //current viewport

  int w = imageWidth_;
  int h = imageHeight_;

  bits = new GLubyte[w * 1* h];

    std::cout<<"Written"<<std::endl;
  //read pixel from frame buffer
  glPixelStorei(GL_PACK_ALIGNMENT, 1); //or glPixelStorei(GL_PACK_ALIGNMENT,4);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    std::cout<<"Writtenf"<<std::endl;
  glReadPixels(0, 0, w, h, format, GL_UNSIGNED_BYTE, bits);

    std::cout<<"Writtenf"<<std::endl;
  cv::Mat tem = cv::Mat(h,w,CV_8UC3);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      for (int curCh = 0; curCh < ch; ++curCh) {
        tem.at<cv::Vec3b>(i, j)[2-curCh] = (unsigned char)255* (bits[(h - i - 1) * 3 * w + j * 1 + curCh]);
      }
    }
  }

  capture =tem;

  //cv::imwrite("result.png", capture);
  tem.release();
  delete[] bits;
}

void OpenGLProgram::CaptureViewPortFloat(cv::Mat& capture, GLenum format, GLuint ch) {

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  GLfloat * bits; //RGB bits
  GLint viewport[4]; //current viewport

  int w = imageWidth_;
  int h = imageHeight_;

  bits = new GLfloat[w * ch * h];

  //read pixel from frame buffer
  glFinish(); //finish all commands of OpenGL
  glPixelStorei(GL_PACK_ALIGNMENT, 1); //or glPixelStorei(GL_PACK_ALIGNMENT,4);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
  glReadPixels(0, 0, w, h, format, GL_FLOAT, bits);

  cv::Mat tem = cv::Mat(h,w,CV_8UC3);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      for (int curCh = 0; curCh < ch; ++curCh) {
        tem.at<cv::Vec3b>(i, j)[2-curCh] = 255*(float) (bits[(h - i - 1) * ch * w + j * ch + curCh]);
      }
    }
  }
//  capture = capImg;

  capture = tem;
  cv::imwrite("result.png", capture);
  delete[] bits;
}
bool OpenGLProgram::initGlwf() {
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window_ = glfwCreateWindow(imageWidth_, imageHeight_, "mum...mumble", nullptr, nullptr);

  if (window_ == NULL) {
    fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    return false;
  }
  return true;
}

void OpenGLProgram::SwapBuffers() {

  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void OpenGLProgram::init() {
  glewExperimental=GL_TRUE;
  initGlwf();
  glfwMakeContextCurrent(window_);
  glewInit();
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
  glClearColor(0.0f, 0.0f, 0.00f, 0.0f);

  glGenVertexArrays(1, &vertexArrayObj_);
  glBindVertexArray(vertexArrayObj_);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

}


void OpenGLProgram::checkLastError() {
  GLenum res = glGetError();
  switch (res) {
  case GL_NO_ERROR:
    std::cerr << "GL_NO_ERROR" << std::endl;
    break;
  case GL_INVALID_ENUM:
    std::cerr << "GL_INVALID_ENUM" << std::endl;
    break;
  case GL_INVALID_VALUE:
    std::cerr << "GL_INVALID_VALUE" << std::endl;
    break;
  case GL_INVALID_OPERATION:
    std::cerr << "GL_INVALID_OPERATION" << std::endl;
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
    break;
  case GL_OUT_OF_MEMORY:
    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
    break;
  case GL_STACK_UNDERFLOW:
    std::cerr << "GL_STACK_UNDERFLOW" << std::endl;
    break;
  case GL_STACK_OVERFLOW:
    std::cerr << "GL_STACK_OVERFLOW" << std::endl;
    break;
  default:
    break;
  }
}

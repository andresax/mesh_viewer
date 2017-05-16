#ifndef MeshViewer_H_
#define MeshViewer_H_

#include "OpenGLProgram.h"
#include "types.hpp"
#include "Mesh.h"
#include "CamParser.h"
#include "ShaderProgram.h"
#include "TransformationController.h"

#include <opencv2/core/core.hpp>
#include <iostream>
#include <fstream>
#include <vector>


class MeshViewer : public OpenGLProgram {
public:
  MeshViewer(std::string namecam);
  virtual ~MeshViewer();

  void run();
  void runSimple(std::string namemesh);
void restartWithNewMesh(const Mesh& mesh);
  //void simpleMeshUpdate();

private:
  void initialize();
  std::string getNumberFrameToStr(int countFrame);
  void initShaders();
  void createVertexArrayBuffer();
  void resetVertexArrayBuffer();
  void refreshVertexArrayBuffer();
  glm::mat4 setCameraParamAndGetMvp(const CameraType &cam);
  void resetMeshInfo();
  
  Mesh mesh_;
  std::vector<cv::Mat> images_;
  CamParser *camParser_;
  TransformationController *cameraTransformationController_;
   //***************shaders*******************************************
  ShaderProgram *depthProgram_;
  ShaderProgram *reprojProgram_;

  //***************shaders variables*********************************
  GLuint vertexBufferObj_, imageArrayBufferObj_, imageElemBufferObj_;
  GLuint framebufferDepth_;
  GLuint depthTexture_;
  GLuint reprojTex_;
  std::string namecam_;
};

#endif /* MeshViewer_H_ */

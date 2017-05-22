#ifndef MeshViewer_H_
#define MeshViewer_H_

#include "Mesh.h"
#include "CamParser.h"
#include <ShaderProgram.h>
#include <OpenGLProgram.h>

#include <opencv2/core/core.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ViewingDataParser.h>
#include <types.hpp>


class MeshViewer : public OpenGLProgram {
public:
  MeshViewer(std::string namecam);
  virtual ~MeshViewer();

  void run();
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
  void loadMesh(std::string path);

Polyhedronmy ptoot;
  
  Mesh mesh_;
  std::vector<cv::Mat> images_;
  CamParser *camParser_;
   //***************shaders*******************************************
  ShaderProgram *depthProgram_;
  ShaderProgram *reprojProgram_;

  ViewerConfiguration c;

  //***************shaders variables*********************************
  GLuint vertexBufferObj_, imageArrayBufferObj_, imageElemBufferObj_;
  GLuint framebufferDepth_;
  GLuint depthTexture_;
  GLuint reprojTex_;
  GLuint vertexBufferObjGrad_;

  std::vector<ViewingTriplet> orderedViewingTriplets_;
};

#endif /* MeshViewer_H_ */

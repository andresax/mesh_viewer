/*
 * MeshSweeper.h
 *
 *  Created on: 01/apr/2015
 *      Author: andrea
 */

#ifndef KITTIVIEWERMESH_MESHSWEEPER_H_
#define KITTIVIEWERMESH_MESHSWEEPER_H_

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


class KittiViewer : public OpenGLProgram {
public:
  KittiViewer(std::string namecam);
  virtual ~KittiViewer();

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

#endif /* KITTIVIEWERMESH_MESHSWEEPER_H_ */

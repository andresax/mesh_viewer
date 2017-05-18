#include <MeshViewer.h>
#include <utilities.hpp>
#include <ReprojectionShaderProgram.h>
#include <DepthShaderProgram.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Configurator.h>
#include <glm.hpp>


MeshViewer::MeshViewer( std::string namecam) {
 Configurator conf(namecam);
 c = conf.parseConfigFile();
  initialize();
}


MeshViewer::~MeshViewer() {
}

void MeshViewer::run() {
  cv::Mat image;

int countframe=0;
  for (auto curTriplet: orderedViewingTriplets_){
 
    if(curTriplet.meshPath.compare("")!=0){//the string is not void
      mesh_.loadFormat(curTriplet.meshPath.c_str(), false);
      resetMeshInfo();
    }


    glm::mat4 mvp = camParser_->getSfmData().camerasList_[curTriplet.cameraId].mvp;
    glm::vec3 curCenter = camParser_->getSfmData().camerasList_[curTriplet.cameraId].center;
    

    cv::Mat curImage;

    if(curTriplet.imageId==-1){
      curImage = cv::imread(camParser_->getSfmData().camerasList_[curTriplet.imageId].cameraPath);
    }

    //************************depth************************
    depthProgram_->setArrayBufferObj(vertexBufferObj_, mesh_.p.size_of_facets() * 3);
    depthProgram_->setUseElementsIndices(false);
    static_cast<DepthShaderProgram *>(depthProgram_)->computeDepthMap(framebufferDepth_, mvp);
    glFinish();

    //************************reprojection**************************
    reprojProgram_->setArrayBufferObj(vertexBufferObj_, mesh_.p.size_of_facets() * 3);
    reprojProgram_->setUseElementsIndices(false);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setCamCenter(curCenter);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setDepthTexture(depthTexture_);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setMvp(mvp);
    //reprojProgram_->populateTex(image);
    reprojProgram_->compute(false);
    glFinish();
    cv::Mat capture;
    CaptureViewPortFloat(capture, GL_RGB, 3);
    SwapBuffers();    

    std::stringstream ss;
    ss<<"render_"<<c.pathOutDir_<<utilities::getFrameNumber(countframe,6)<<".png";
    cv::imwrite(ss.str().c_str(),capture);

      std::cout << "Iteration num. " << countframe << " done!" << std::endl;
      countframe++;
  }
  image.release();
}


void MeshViewer::restartWithNewMesh(const Mesh& mesh) {

  mesh_.clear();
  mesh_ = mesh;
  resetMeshInfo();
}

void MeshViewer::initShaders() {
  //************************depth********************************
  std::cout << "DepthShaderProgram init...";
  depthProgram_->initializeProgram();
  static_cast<DepthShaderProgram *>(depthProgram_)->initializeFramebufAndTex(framebufferDepth_, depthTexture_);
  std::cout << "DONE" << std::endl;

  //************************reprojection**************************
  std::cout << "ReprojectionShaderProgram init...";
  reprojProgram_->initializeProgram();
  reprojProgram_->initTex();
  static_cast<ReprojectionShaderProgram *>(reprojProgram_)->initializeFramebufAndTex(reprojTex_);
  std::cout << "DONE" << std::endl;
}

void MeshViewer::createVertexArrayBuffer() {
  glGenBuffers(1, &vertexBufferObj_);
  resetVertexArrayBuffer();

}

void MeshViewer::resetMeshInfo() {

  mesh_.updateMeshData(false, false);
  mesh_.resetSimplexIndices();

  resetVertexArrayBuffer();
}

void MeshViewer::resetVertexArrayBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj_);
  std::vector<glm::vec3> verticesUnwrapped;

  for (Facet_iterator itFac = mesh_.p.facets_begin(); itFac != mesh_.p.facets_end(); itFac++) {
    Halfedge_handle h0, h1, h2;
    h0 = itFac->halfedge();
    h1 = h0->next();
    h2 = h1->next();

    Vertex_handle v0, v1, v2;
    v0 = h0->vertex();
    v1 = h1->vertex();
    v2 = h2->vertex();

    verticesUnwrapped.push_back(glm::vec3(v0->point().x(), v0->point().y(), v0->point().z()));
    verticesUnwrapped.push_back(glm::vec3(v1->point().x(), v1->point().y(), v1->point().z()));
    verticesUnwrapped.push_back(glm::vec3(v2->point().x(), v2->point().y(), v2->point().z()));
  }

  glBufferData(GL_ARRAY_BUFFER, 3 * mesh_.p.size_of_facets() * sizeof(glm::vec3), &verticesUnwrapped[0], GL_STATIC_DRAW);
}




void MeshViewer::initialize() {


  vertexBufferObj_  = imageElemBufferObj_ = -1;
  framebufferDepth_ = depthTexture_ = -1;
  reprojTex_ = -1;

  ViewingDataParser vdp_(c.correspondencesPath_);
  vdp_.parseFile();
  orderedViewingTriplets_ = vdp_.getOrderedViewingTriplets();

  camParser_ = new CamParser(c.camerasFilePath_.c_str());
  std::cout<<c.camerasFilePath_<<std::endl;
  camParser_->parseFile();
  imageHeight_ = camParser_->getSfmData().imageHeight_;
  imageWidth_ = camParser_->getSfmData().imageWidth_;

    std::cout<<"imageHeight_ " << imageHeight_ <<std::endl;
    std::cout<<"imageWidth_ " << imageWidth_ <<std::endl;


  init();
  createVertexArrayBuffer();
  initShaders();


  depthProgram_ = new DepthShaderProgram(imageWidth_, imageHeight_);
  reprojProgram_ = new ReprojectionShaderProgram(imageWidth_, imageHeight_);

}
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
#include <Polyhedron_scan_COFF.h>


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
 
    if(curTriplet.meshPath.compare("") != 0){//the string is not void
      //mesh_.loadFormat(curTriplet.meshPath.c_str(), false);
      loadMesh(curTriplet.meshPath);
      resetMeshInfo();
    }


    glm::mat4 mvp = camParser_->getSfmData().camerasList_[curTriplet.cameraId].mvp;
    glm::vec3 curCenter = camParser_->getSfmData().camerasList_[curTriplet.cameraId].center;
    

    cv::Mat curImage;

    if(curTriplet.imageId==-1){
      curImage = cv::imread(camParser_->getSfmData().camerasList_[curTriplet.imageId].cameraPath);
    }

    //************************depth************************
    depthProgram_->setArrayBufferObj(vertexBufferObj_, ptoot.size_of_facets() * 3);
    depthProgram_->setUseElementsIndices(false);
    static_cast<DepthShaderProgram *>(depthProgram_)->computeDepthMap(framebufferDepth_, mvp);
    glFinish();

    //************************reprojection**************************
    reprojProgram_->setArrayBufferObj(vertexBufferObj_, ptoot.size_of_facets() * 3);
    reprojProgram_->setUseElementsIndices(false);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setCamCenter(curCenter);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setDepthTexture(depthTexture_);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setMvp(mvp);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setVertexArrayObj(vertexArrayObj_);
    static_cast<ReprojectionShaderProgram *>(reprojProgram_)->setVertexArrayGradObj(vertexBufferObjGrad_);
    //reprojProgram_->populateTex(image);
    reprojProgram_->compute(false);
    glFinish();
    cv::Mat capture;
    CaptureViewPortFloat(capture, GL_RGB, 3);
    SwapBuffers();    

    std::stringstream ss;
    ss<<c.pathOutDir_<<utilities::getFrameNumber(countframe,6)<<".png";
    cv::imwrite(ss.str().c_str(),capture);

    std::cout << "Iteration num. " << countframe << " done!  Image saved in " << ss.str()<< std::endl;
    countframe++;
  }
  image.release();
}


void MeshViewer::loadMesh(std::string path) {

std::cout << "MeshViewer::loadMesh"<< std::flush;
  std::ifstream f(path);
  Polyhedronmy p;

  Polyhedron_scan_COFF scanner(f);
  ptoot.delegate(scanner);

  int countR = 0, countG = 0, countB = 0, count = 0;
  for (Facet_iteratormy facet = ptoot.facets_begin(); facet != ptoot.facets_end(); facet++) {
    if (facet->color == CGAL::RED) {
      countR++;
      facet->label.set_label(classLabel::GROUND);
    } else if (facet->color == CGAL::GREEN) {
      facet->label.set_label(classLabel::CAR);
      countG++;
    } else if (facet->color == CGAL::BLUE) {
      facet->label.set_label(classLabel::WALL);
      countB++;
    } else {
      facet->label.set_label(classLabel::UNDEFINED);
      count++;
    }

  }

 // mesh_.loadAsTriangleSoup(path.c_str());
  //curToBeRefined.loadFormat("/home/andrea/workspaceC/semanticMeshReconstruction/cubeNewOpt_noColor.off", false);

 /* Facet_iteratormy f1 = p.facets_begin();
  for (Facet_iterator f = mesh_.p.facets_begin(); f != mesh_.p.facets_end() && f1 != p.facets_end(); f++, f1++) {
    f->id = static_cast<int>(f1->label.l());
  }
*/
std::cout << " DONE"<< std::endl;
}
/*void MeshViewer::restartWithNewMesh(const Mesh& mesh) {

  resetMeshInfo();
}
*/
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
  glGenBuffers(1, &vertexBufferObjGrad_);
  resetVertexArrayBuffer();

}

void MeshViewer::resetMeshInfo() {

  resetVertexArrayBuffer();
}

void MeshViewer::resetVertexArrayBuffer() {
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj_);
  std::vector<glm::vec3> verticesUnwrapped;

  for (Facet_iteratormy itFac = ptoot.facets_begin(); itFac != ptoot.facets_end(); itFac++) {
    Halfedge_handlemy h0, h1, h2;
    h0 = itFac->halfedge();
    h1 = h0->next();
    h2 = h1->next();

    Vertex_handlemy v0, v1, v2;
    v0 = h0->vertex();
    v1 = h1->vertex();
    v2 = h2->vertex();

    verticesUnwrapped.push_back(glm::vec3(v0->point().x(), v0->point().y(), v0->point().z()));
    verticesUnwrapped.push_back(glm::vec3(v1->point().x(), v1->point().y(), v1->point().z()));
    verticesUnwrapped.push_back(glm::vec3(v2->point().x(), v2->point().y(), v2->point().z()));
  }
  glBufferData(GL_ARRAY_BUFFER, 3 * ptoot.size_of_facets() * sizeof(glm::vec3), &verticesUnwrapped[0], GL_STATIC_DRAW);

  std::vector<glm::vec4> verticesUnwrapped2;
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjGrad_);
  for (Facet_iteratormy itFac = ptoot.facets_begin(); itFac != ptoot.facets_end(); itFac++) {
    verticesUnwrapped2.push_back(glm::vec4(itFac->color.r(), itFac->color.g(), itFac->color.b(), 1.0));
    verticesUnwrapped2.push_back(glm::vec4(itFac->color.r(), itFac->color.g(), itFac->color.b(), 1.0));
    verticesUnwrapped2.push_back(glm::vec4(itFac->color.r(), itFac->color.g(), itFac->color.b(), 1.0));
  }
  glBufferData(GL_ARRAY_BUFFER, verticesUnwrapped2.size() * sizeof(glm::vec4), &verticesUnwrapped2[0], GL_DYNAMIC_DRAW);


  std::cout<<"MeshViewer::resetVertexArrayBuffer"<<std::endl;
  std::cout<<"verticesUnwrapped.size()="<<verticesUnwrapped.size()<<std::endl;
  std::cout<<"verticesUnwrapped2.size()="<<verticesUnwrapped2.size()<<std::endl;
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

  depthProgram_ = new DepthShaderProgram(imageWidth_, imageHeight_);
  reprojProgram_ = new ReprojectionShaderProgram(imageWidth_, imageHeight_);

  init();
  createVertexArrayBuffer();
  initShaders();

}
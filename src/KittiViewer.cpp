#include <KittiViewer.h>
#include <utilities.hpp>
#include <ReprojectionShaderProgram.h>
#include <DepthShaderProgram.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm.hpp>


KittiViewer::KittiViewer( std::string namecam) {
 namecam_ = namecam;
  //mesh_.loadFormat("/home/andrea/workspaceC/incremental_dense_reconstruction/Results/Incremental_0095_weight140_536_20_60_1_3_0.07_3_0.9_3_5_10_5e-15_0.005/Mesh_240.off", false);
  //mesh_.removeSelfIntersections();
  //mesh_.smooth(0.5,0);
  //mesh_.smooth(0.5,0);
  initialize();

}


KittiViewer::~KittiViewer() {
  }



void KittiViewer::run() {
  cv::Mat image;
  glm::mat4 mvp;
  int countFrame=0;
  int countFramess=0;
  cv::Mat capture;  
  cv::Mat last;  
  bool newmesh=false;
  bool merged=false;
  /*float totl=0.0;
  glm::vec3 lastc;
  for (int idx1 = 1; idx1 < camParser_->getNumCameras();  idx1+=2) {
    setCameraParamAndGetMvp(camParser_->getCamerasList()[idx1]);
    glm::vec3 curCenter = cameraTransformationController_->getCameraCenter();
    if(idx1>1){
      totl = totl + glm::length(lastc - curCenter);
    }
    lastc = curCenter;

  }*/
  
  /*float curlength=0.0;
  setCameraParamAndGetMvp(camParser_->getCamerasList()[0]);
  lastc = cameraTransformationController_->getCameraCenter();*/

  //int last=-1;
  int offset = 60;
  for (int idx1 = 0; idx1 < camParser_->getNumCameras(); idx1+=2) {
    mvp = setCameraParamAndGetMvp(camParser_->getCamerasList()[idx1+1]);
    glm::vec3 curCenter = cameraTransformationController_->getCameraCenter();
    image = cv::imread(camParser_->getCamerasPaths()[idx1]);

    int cur = idx1+offset;
    std::stringstream ss1;
    if(idx1==0)
      ss1<<"/home/andrea/workspaceC/incremental_dense_reconstruction/Results/Incremental_0095_weight140_536_20_60_1_3_0.07_3_0.9_3_5_10_5e-15_0.005/Mesh_curToBeRefined_count240.off";
    else if(cur>240)
      ss1<<"/home/andrea/workspaceC/incremental_dense_reconstruction/Results/Incremental_0095_weight140_536_20_60_1_3_0.07_3_0.9_3_5_10_5e-15_0.005/Mesh_curToBeRefined_count"<<cur<<".off";
    
    std::ifstream infile(ss1.str());
    if(infile.good()){
      for (int i = 0; i<(merged? 10:0); ++i){
        /*std::stringstream ss2;
        ss2<<"0095IncrRec/"<<utilities::getFrameNumber(++countFrame)<<".png";
        cv::imwrite(ss2.str().c_str(),last);*/
        std::stringstream ss2,ss3;
        ss2<<"0095IncrRec/"<<utilities::getFrameNumber(++countFrame)<<".png";
        ss3<<"/home/andrea/Desktop/Datasets/KITTI/2011_09_26/2011_09_26_drive_0095_sync/image_02/data/"<<utilities::getFrameNumber(countFramess)<<".png";
        //cv::imwrite(ss3.str().c_str(),capture);

        //cv::imwrite(ss2.str().c_str(),cv::imread(ss3.str().c_str()));
        cv::imwrite(ss2.str().c_str(),cv::imread(ss3.str().c_str()));
      }
      std::cout<<ss1.str()<<std::endl;
      //mesh_.loadFormat(ss1.str().c_str(), false);
      resetMeshInfo();
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
      reprojProgram_->populateTex(image);
      reprojProgram_->compute(false);
      glFinish();
      CaptureViewPortFloat(capture, GL_RGB, 3);
      SwapBuffers();    

      for (int i = 0; i < 10; ++i){
        /*std::stringstream ss2;
        ss2<<"0095IncrRec/"<<utilities::getFrameNumber(++countFrame)<<".png";
        cv::imwrite(ss2.str().c_str(),capture);*/
        std::stringstream ss2,ss3;
        ss2<<"0095IncrRec/"<<utilities::getFrameNumber(++countFrame)<<".png";
        ss3<<"/home/andrea/Desktop/Datasets/KITTI/2011_09_26/2011_09_26_drive_0095_sync/image_02/data/"<<utilities::getFrameNumber(countFramess)<<".png";
        //cv::imwrite(ss2.str().c_str(),capture);
        cv::imwrite(ss2.str().c_str(),cv::imread(ss3.str().c_str()));
      }
      std::cout << "Iteration num. " << idx1 << " done!" << std::endl;
      newmesh=true;
    } 
    infile.close();


    std::stringstream ss;
    //int cur = floor(static_cast<float>(idx1)/2000.0 *891.0)+15;
    //int cur = floor(curlength/totl *930.0)+10;
    if(idx1==0)
      ss<<"/home/andrea/workspaceC/incremental_dense_reconstruction/Results/Incremental_0095_weight140_536_20_60_1_3_0.07_3_0.9_3_5_10_5e-15_0.005/Mesh_240.off";
    else if(cur>240)
      ss<<"/home/andrea/workspaceC/incremental_dense_reconstruction/Results/Incremental_0095_weight140_536_20_60_1_3_0.07_3_0.9_3_5_10_5e-15_0.005/Mesh_"<<cur<<".off";
    std::cout<<"------floor"<<cur<<std::endl;
    
    std::ifstream infile2(ss.str());
    if(infile2.good() ){
      std::cout<<ss.str()<<std::endl;
      //mesh_.loadFormat(ss.str().c_str(), false);
      ////mesh_.smooth(0.5,0);
      //mesh_.smooth(0.5,0);
      resetMeshInfo();
      //last = cur;

    }
    infile2.close();


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
    reprojProgram_->populateTex(image);
    reprojProgram_->compute(false);
    glFinish();

    SwapBuffers();    
    CaptureViewPortFloat(capture, GL_RGB, 3);

    std::cout << "newmesh. " << newmesh << " done!" << std::endl;
    std::cout << "newmesh?10:1. " << (newmesh==true ? 10 : 1) << " done!" << std::endl;
    for (int i = 0; i < (newmesh ? 10 : 1); ++i){
      std::stringstream ss2,ss3;
      ss2<<"0095IncrRec/"<<utilities::getFrameNumber(++countFrame)<<".png";
      ss3<<"/home/andrea/Desktop/Datasets/KITTI/2011_09_26/2011_09_26_drive_0095_sync/image_02/data/"<<utilities::getFrameNumber(countFramess)<<".png";
     // cv::imwrite(ss3.str().c_str(),capture);
        cv::imwrite(ss2.str().c_str(),cv::imread(ss3.str().c_str()));
    }
    newmesh=false;
    merged=true;
    countFramess++;
    std::cout << "Iteration num. " << idx1 << " done!" << std::endl;
    last = capture;
  }  

  image.release();
}


void KittiViewer::runSimple(std::string namemesh) {
  cv::Mat image;
  glm::mat4 mvp;
  int countFrame=0;
  int countFramess=0;
  cv::Mat capture;  
  cv::Mat last;  
  bool merged=false;
//TODO change hardcoded mesh file name
  mesh_.loadFormat(namemesh.c_str(), false);
  std::cout<<namemesh<<std::endl;
  resetMeshInfo();
 
  for (int idx1 = 0; idx1 < camParser_->getNumCameras(); idx1++) {
    std::stringstream ss5;
    //ss5<<"/home/andrea/workspaceC/kittiviewer/AlsoRenders/models/frame_"<<utilities::getFrameNumber(idx1)<<".off";
    //mesh_.loadFormat(ss5.str().c_str(), false);
    //resetMeshInfo();

    mvp = setCameraParamAndGetMvp(camParser_->getCamerasList()[idx1]);
    glm::vec3 curCenter = cameraTransformationController_->getCameraCenter();
    //image = cv::imread(camParser_->getCamerasPaths()[idx1]);
    //image = cv::Mat();
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
    CaptureViewPortFloat(capture, GL_RGB, 3);
    SwapBuffers();    

    std::stringstream ss2;
    ss2<<"render_"<<utilities::getFrameNumber(idx1)<<".png";
    cv::imwrite(ss2.str().c_str(),capture);
      
      std::cout << "Iteration num. " << idx1 << " done!" << std::endl;
  }
  image.release();
}


void KittiViewer::restartWithNewMesh(const Mesh& mesh) {

  mesh_.clear();
  mesh_ = mesh;
  resetMeshInfo();
}

void KittiViewer::initShaders() {
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

void KittiViewer::createVertexArrayBuffer() {
  glGenBuffers(1, &vertexBufferObj_);
  resetVertexArrayBuffer();

}

void KittiViewer::resetMeshInfo() {

  mesh_.updateMeshData(false, false);
  mesh_.resetSimplexIndices();

  resetVertexArrayBuffer();
}

void KittiViewer::resetVertexArrayBuffer() {
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



glm::mat4 KittiViewer::setCameraParamAndGetMvp(const CameraType &cam) {
  glm::mat3 r = cam.rotation;
  /*r = glm::transpose(cam.rotation);
  glm::vec3 t = - cam.center * (r);*/
 glm::vec3 t =  cam.translation;
  cameraTransformationController_->setIntrinsicParameters(cam.intrinsics[0][0], cam.intrinsics[1][1], cam.intrinsics[0][2], cam.intrinsics[1][2]);
  cameraTransformationController_->setExtrinsicParameters((r), t);
  glm::mat4 mvp = cameraTransformationController_->getMvpMatrix();
  return mvp;
}



void KittiViewer::initialize() {

  vertexBufferObj_  = imageElemBufferObj_ = -1;
  framebufferDepth_ = depthTexture_ = -1;
  reprojTex_ = -1;
  imageHeight_ = 1200;
  imageWidth_ = 1600;

  ///camParser_ = new CamParser("/home/andrea/workspaceC/kittiviewer/AlsoRenders/cam_poses.txt");std::string namecam
  camParser_ = new CamParser(namecam_.c_str());
  std::cout<<namecam_<<std::endl;
  camParser_->parseFile();
  std::cout<<"numcam "<<camParser_->getNumCameras()<<std::endl;

  depthProgram_ = new DepthShaderProgram(imageWidth_, imageHeight_);
  reprojProgram_ = new ReprojectionShaderProgram(imageWidth_, imageHeight_);
  
  cameraTransformationController_ = new TransformationController(static_cast<float>(imageWidth_), static_cast<float>(imageHeight_));

  init();
  createVertexArrayBuffer();
  initShaders();
}
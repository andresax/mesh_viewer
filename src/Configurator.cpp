#include <Configurator.h>
#include <utilities.hpp>
#include <boost/filesystem.hpp>

Configurator::Configurator(const std::string &path) {
  file_.open(path.c_str());
  if (file_.is_open())
    std::cout << path << " opened" << std::endl;
  else
    std::cout << "Error reading " << path << std::endl;
}

Configurator::~Configurator() {
}

ConfigurationSemanticReconstruction Configurator::parseConfigFile() {

  std::cout << "Configurator::parseConfigFile ... " << std::flush;
  utilities::readLineAndStore(file_, c.fileOpenMVG_);

  createDirRes();

  if (c.loadInitRec_){
    c.refinementConfig_.pathInitialMesh_ = c.pathOutput_+c.pathOutputManifoldReconstruction_;
  }else if (c.loadInitRecWithClasses_){
    c.refinementConfig_.pathInitialMesh_ = c.pathOutput_+c.pathOutputInputManifoldReconstructionWithClasses_;
  }else{
    c.refinementConfig_.pathInitialMesh_ = "";
  }
  c.manifoldReconstructionConfig_.pathOutputManifoldReconstruction_ = c.pathOutput_+c.pathOutputManifoldReconstruction_;
  std::cout << "DONE" << std::endl;
  return c;
}

void Configurator::createDirRes() {

  std::cout << "Configurator::createDirRes  ..." << std::flush;
  std::stringstream s;
  std::string base = "Results/";
  s << base << "Incremental_" << optionalNameDir << "_" << c.manifoldReconstructionConfig_.probOrVoteThreshold << "_"
      << c.manifoldReconstructionConfig_.maxDistanceCamFeature << "_" << c.manifoldReconstructionConfig_.w_1 << "_" << c.manifoldReconstructionConfig_.w_2
      << "_" << c.manifoldReconstructionConfig_.w_3 << "_" << c.manifoldReconstructionConfig_.stepX_ << "_" << c.manifoldReconstructionConfig_.stepY_ << "_"
      << c.manifoldReconstructionConfig_.stepZ_ << "_" << c.manifoldReconstructionConfig_.inX << "_" << c.manifoldReconstructionConfig_.finX << "_"
      << c.manifoldReconstructionConfig_.inY << "_" << c.manifoldReconstructionConfig_.finY << "_" << c.manifoldReconstructionConfig_.inZ << "_"
      << c.manifoldReconstructionConfig_.finZ << "_" << c.refinementConfig_.lambdaSemantic_ << "_" << c.refinementConfig_.lambdaPhoto_ << "_"
      << c.refinementConfig_.lambdaSmooth_ << "_" << c.refinementConfig_.maxDistanceCamFeatureRef << "_" << c.refinementConfig_.numIt_ << "_"
      << c.refinementConfig_.ensureedgemax_ << "_" << c.refinementConfig_.ensureedgeit_<<"/";

  boost::filesystem::path dirbase(base.c_str());
  boost::filesystem::create_directories(dirbase);

  boost::filesystem::path dir(s.str().c_str());
  boost::filesystem::create_directories(dir);

  c.pathOutput_ = s.str();
  c.refinementConfig_.pathOutputRefinement = c.pathOutput_;
  std::cout << "DONE" << std::endl;
}

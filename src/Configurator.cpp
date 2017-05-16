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

ViewerConfiguration Configurator::parseConfigFile() {

  std::cout << "Configurator::parseConfigFile ... " << std::flush;
  utilities::readLineAndStore(file_, c.camerasFilePath_);
  utilities::readLineAndStore(file_, c.correspondencesPath_);
  utilities::readLineAndStore(file_, c.pathOutDir_);
  
  createDirRes();

  return c;
}

void Configurator::createDirRes() {
  std::cout << "Configurator::createDirRes  ..." << std::flush;
  boost::filesystem::path dir(c.pathOutDir_.str().c_str());
  boost::filesystem::create_directories(dir);
  std::cout << "DONE" << std::endl;
}

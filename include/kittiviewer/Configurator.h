#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <types_config.hpp>

#include <string>
#include <fstream>
#include <iostream>

class Configurator {
  public:
    Configurator(const std::string &path);
    virtual ~Configurator();
    ConfigurationSemanticReconstruction parseConfigFile();

  void setOptionalNameDir(const std::string& optionalNameDir) {
    this->optionalNameDir = optionalNameDir;
  }

  private:
    void createDirRes();

    std::string optionalNameDir;

    ConfigurationSemanticReconstruction c;
    std::ifstream file_;
};

#endif /* CONFIGURATOR_H_ */

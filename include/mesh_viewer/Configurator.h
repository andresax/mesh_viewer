#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <types.hpp>

#include <string>
#include <fstream>
#include <iostream>

class Configurator {
  public:
    Configurator(const std::string &path);
    virtual ~Configurator();
    ViewerConfiguration parseConfigFile();

  private:
    void createDirRes();

    ViewerConfiguration c;
    std::ifstream file_;
};

#endif /* CONFIGURATOR_H_ */

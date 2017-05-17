//  Copyright 2014 Andrea Romanoni

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "MeshViewer.h"

int main(int argc, char **argv) {
  MeshViewer k(argv[1]);
  k.run();
  return 0;
}


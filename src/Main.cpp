//  Copyright 2014 Andrea Romanoni

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "MeshViewer.h"

int main(int argc, char **argv) {
  MeshViewer k(argv[2]);
  k.runSimple(argv[1]);
  return 0;
}


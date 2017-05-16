//  Copyright 2014 Andrea Romanoni

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "KittiViewer.h"

int main(int argc, char **argv) {
  KittiViewer k(argv[2]);
  k.runSimple(argv[1]);
  return 0;
}


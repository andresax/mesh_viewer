#include "utilities.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace utilities {

double distanceEucl(cv::Point3d p1, cv::Point3d p2) {
  return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}

double distanceEucl(double x1, double x2, double y1, double y2, double z1, double z2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

std::string getFrameNumber(int curFrame, int digitIdxLength) {
  std::ostringstream curNumber;
  if (digitIdxLength > 0) {
    int n = curFrame;
    int curNumOfDigit = curFrame == 0 ? 1 : 0;
    while (n > 0) {
      n /= 10;
      ++curNumOfDigit;
    }
    while (curNumOfDigit < digitIdxLength) {
      curNumber << "0";
      curNumOfDigit++;
    }
  }
  curNumber << curFrame;
  return curNumber.str();
}

void writeObj(std::vector<glm::vec3> &vertices, std::string path) {
  std::ofstream fileOut(path);
  for (auto v : vertices) {
    fileOut << "v " << v.x << " " << v.y << " " << v.z << " " << std::endl;
  }

  for (int curF = 0; curF < vertices.size(); curF += 3) {
    fileOut << "f " << curF + 1 << " " << curF + 2 << " " << curF + 3 << " " << std::endl;
  }

  fileOut.close();
}


glm::mat3 rotX(float alpha) {
  glm::mat3 rot; //set identity matrix
  rot[1][1] = cos(alpha);
  rot[1][2] = -sin(alpha);
  rot[2][1] = sin(alpha);
  rot[2][2] = cos(alpha);
  return rot;
}

glm::mat3 rotY(float alpha) {
  glm::mat3 rot; //set identity matrix
  rot[0][0] = cos(alpha);
  rot[0][2] = sin(alpha);
  rot[2][0] = -sin(alpha);
  rot[2][2] = cos(alpha);
  return rot;
}

void printMatrix(glm::mat4 matrix) {
  for (int curRow = 0; curRow < 4; ++curRow) {
    for (int curCol = 0; curCol < 4; ++curCol) {
      std::cout << matrix[curRow][curCol] << " ";
    }
    std::cout << std::endl;
  }
}
void printMatrix(const std::string message, glm::mat4 matrix) {
  std::cout << message << std::endl;
  for (int curRow = 0; curRow < 4; ++curRow) {
    for (int curCol = 0; curCol < 4; ++curCol) {
      std::cout << matrix[curRow][curCol] << " ";
    }
    std::cout << std::endl;
  }
}
void printMatrix(glm::mat3 matrix) {
  for (int curRow = 0; curRow < 3; ++curRow) {
    for (int curCol = 0; curCol < 3; ++curCol) {
      std::cout << matrix[curRow][curCol] << " ";
    }
    std::cout << std::endl;
  }
}

void printMatrix(std::string message, glm::mat3 matrix) {
  std::cout << message << std::endl;
  for (int curRow = 0; curRow < 3; ++curRow) {
    for (int curCol = 0; curCol < 3; ++curCol) {
      std::cout << matrix[curRow][curCol] << " ";
    }
    std::cout << std::endl;
  }
}

void printMatrix(glm::vec3 vector) {
  for (int curIdx = 0; curIdx < 3; ++curIdx) {
    std::cout << vector[curIdx] << " ";
  }
  std::cout << std::endl;
}

void printMatrix(std::string message, glm::vec4 vector) {
  std::cout << message << std::endl;
  printMatrix(vector);
}
void printMatrix(glm::vec4 vector) {
  for (int curIdx = 0; curIdx < 4; ++curIdx) {
    std::cout << vector[curIdx] << " ";
  }
  std::cout << std::endl;
}

void printMatrix(std::string message, glm::vec3 vector) {
  std::cout << message << std::endl;
  for (int curIdx = 0; curIdx < 3; ++curIdx) {
    std::cout << vector[curIdx] << " ";
  }
  std::cout << std::endl;
}

glm::mat3 rotZ(float alpha) {
  glm::mat3 rot; //set identity matrix
  rot[0][0] = cos(alpha);
  rot[0][1] = -sin(alpha);
  rot[1][0] = sin(alpha);
  rot[1][1] = cos(alpha);
  return rot;
}


void readLineAndStore(std::ifstream &configFile, bool &value) {
  std::string line;
  int valueRead;
  std::getline(configFile, line);

  std::istringstream iss(line);
  iss >> valueRead;
  if (valueRead == 0) {
    value = false;
  } else {
    value = true;
  }
}

void readLineAndStore(std::ifstream &configFile, int &value) {
  std::string line;
  std::getline(configFile, line);

  std::istringstream iss(line);
  iss >> value;
}

void readLineAndStore(std::ifstream &configFile, double &value) {
  std::string line;
  std::getline(configFile, line);

  std::istringstream iss(line);
  iss >> value;
}
void readLineAndStore(std::ifstream &configFile, float &value) {
  std::string line;
  std::getline(configFile, line);

  std::istringstream iss(line);
  iss >> value;
}

void readLineAndStore(std::ifstream &configFile, std::string &value) {
  std::string line;
  std::getline(configFile, line);

  std::istringstream iss(line);
  iss >> value;
  if (value.at(0) == '#') {
    value = std::string("");
  }
}

}


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

void convertToMvp2(CameraType &cam, glm::mat4 &mvpOut) {

  glm::mat4 modelViewMatrix, projectionMatrix;
  computeModelViewMatrix2(cam.rotation, cam.translation, modelViewMatrix);
  computeProjectionMatrix2(cam.intrinsics, cam.imageHeight, cam.imageWidth, projectionMatrix);
  mvpOut = projectionMatrix * modelViewMatrix;
}

void computeProjectionMatrix2(glm::mat3 &intrinsics, int h, int w, glm::mat4 &projectionMatrixOut) {

  float N = 0.001;
  float F = 100.0;

  glm::mat4 persp = glm::mat4(0.0);
  persp[0][0] = intrinsics[0][0];
  persp[0][1] = 0.0;
  persp[0][2] = intrinsics[0][2];
  persp[1][1] = intrinsics[1][1];
  persp[1][2] = intrinsics[1][2];
  persp[2][2] = -(N + F);
  persp[2][3] = N * F;
  persp[3][2] = 1.0;

  double L = 0;
  double R = w;
  double B = 0;
  double T = h;
  glm::mat4 ortho = glm::mat4(0.0);

  int dino = 1;/*to change the y sign if dino file format is used*/
  ortho[0][0] = 2.0 / (R - L);
  ortho[0][3] = -(R + L) / (R - L);
  ortho[1][1] = (1 - 2 * dino) * 2.0 / (T - B);
  ortho[1][3] = -(1 - 2 * dino) * (T + B) / (T - B);
  ortho[2][2] = (-2.0 / (F - N));
  ortho[2][3] = (-(F + N) / (F - N));
  ortho[3][3] = 1.0;
  projectionMatrixOut = glm::transpose(persp * ortho);
}

void computeModelViewMatrix2(glm::mat3 &rotation, glm::vec3 &translation, glm::mat4 &modelViewMatrixOut) {

  glm::mat4 modelMatrix = glm::mat4();
  glm::mat4 viewMatrix = glm::mat4();

  viewMatrix[0][0] = rotation[0][0];
  viewMatrix[0][1] = rotation[1][0];
  viewMatrix[0][2] = rotation[2][0];
  viewMatrix[1][0] = rotation[0][1];
  viewMatrix[1][1] = rotation[1][1];
  viewMatrix[1][2] = rotation[2][1];
  viewMatrix[2][0] = rotation[0][2];
  viewMatrix[2][1] = rotation[1][2];
  viewMatrix[2][2] = rotation[2][2];
  viewMatrix[3][0] = translation.x;
  viewMatrix[3][1] = translation.y;
  viewMatrix[3][2] = translation.z;
  viewMatrix[0][3] = 0.0;
  viewMatrix[1][3] = 0.0;
  viewMatrix[2][3] = 0.0;
  viewMatrix[3][3] = 1.0;

  modelViewMatrixOut = viewMatrix * modelMatrix;
}

void computeExtrinsicsHZ(const glm::mat3& rotation, const glm::vec3& traslation, glm::mat4& extrinsics) {
  extrinsics[0][0] = rotation[0][0];
  extrinsics[0][1] = rotation[0][1];
  extrinsics[0][2] = rotation[0][2];
  extrinsics[1][0] = rotation[1][0];
  extrinsics[1][1] = rotation[1][1];
  extrinsics[1][2] = rotation[1][2];
  extrinsics[2][0] = rotation[2][0];
  extrinsics[2][1] = rotation[2][1];
  extrinsics[2][2] = rotation[2][2];
  extrinsics[0][3] = traslation[0];
  extrinsics[1][3] = traslation[1];
  extrinsics[2][3] = traslation[2];
  extrinsics[3][3] = 1.0;
}
}


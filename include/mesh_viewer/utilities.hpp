/*
 * utilities.hpp
 *
 *  Created on: 21/gen/2015
 *      Author: andrea
 */

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include <string>

namespace utilities {


double distanceEucl(cv::Point3d p1, cv::Point3d p2);

double distanceEucl(double x1, double x2, double y1, double y2, double z1, double z2);

std::string getFrameNumber(int curFrame, int digitIdxLength);


void writeObj(std::vector<glm::vec3> &vertices, std::string path);


void printMatrix(glm::mat4 matrix);
void printMatrix(std::string message, glm::mat4 matrix);
void printMatrix(glm::mat3 matrix);
void printMatrix(std::string message, glm::mat3 matrix);
void printMatrix(glm::vec3 vector);
void printMatrix(std::string message, glm::vec3 vector);
void printMatrix(glm::vec4 vector);
void printMatrix(std::string message, glm::vec4 vector);


glm::mat3 rotX(float alpha);

glm::mat3 rotY(float alpha);

glm::mat3 rotZ(float alpha);


void readLineAndStore(std::ifstream &configFile, bool &value);
void readLineAndStore(std::ifstream &configFile, int &value) ;
void readLineAndStore(std::ifstream &configFile, double &value);
void readLineAndStore(std::ifstream &configFile, float &value);
void readLineAndStore(std::ifstream &configFile, std::string &value);
}  // namespace utils


#endif /* UTILITIES_HPP_ */

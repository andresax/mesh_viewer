#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <string>
#include <opencv2/core/core.hpp>

#include <glm.hpp>
#include <Eigen/Core>
//This header includes some useful data structure to manage the configuration parameters

struct CameraType {
    glm::mat3 intrinsics;
    glm::mat3 rotation;
    glm::vec3 translation;
    glm::mat4 cameraMatrix;
    //camera center
    glm::vec3 center;
};


typedef struct {
  glm::vec3 point;
  float ncc;
  int idx1;
  int idx2;
}PointNccSweep;


struct PointParser {
    float x;
    float y;
    float z;

    int R;
    int G;
    int B;

    //position of the feature in the corresponding image;
    //the center of the image plane is the origin
    std::vector<float> viewingCamerasX;
    std::vector<float> viewingCamerasY;

    std::vector<int> viewingCamerasIndices;
};



typedef struct  {
    std::string folderImage;
    std::string baseNameImage;
    std::string imageExtension;
    int idxFirstFrame;
    int digitIdxLength; //if 0 no padding
    int idxLastFrame;
    int downsampleRate;
    int imageH;
    int imageW;
} VideoConfig;


typedef struct {
    int keyFramePeriod;
    int keyFramePeriodIteration;
    double cannyHighThreshold;
    int downsamplePeriod;
    int maxGaussNewtonIteration;
    int minDistBetweenTrackedPoints;
    double maxEpipolarDist;
    int minTrackLength;
    int manifoldPeriod;
    bool inverseConicEnabled;
    double probOrVoteThreshold;
    int edgePointEnabled;
    int firstGrowingFrame;
    double maxDistanceCamFeature;
    bool enableSuboptimalPolicy;
    int suboptimalMethod;
} SpaceCarvingConfig;


typedef struct {
    std::string pathToSave;
    std::string pathLog;
    std::string pathLogPoints;
    std::string pathStats;
    std::string pathStatsManifold;
    bool enableSaveReconstr;
    bool enableSaveShrink;
} OutputSpaceCarving;


typedef struct {
  std::string pathInitMesh;
  std::string pathCamsPose;
  float thresholdNCC;
  float kSweepingDistance;
  float numPlanes;
  int windowNCC;
  int windowLocalMaxima;
  int rowGrid;
  int colGrid;
  int maxPointsPerCell;
} SweepConfig;

typedef struct {
  std::string pathInitPoints;
  std::string pathCamsPose;
} ManifoldRecConfig;

typedef struct {
    std::string pathToSave;
    std::string pathFirstMesh;
    std::string nameFirstMesh;
    std::string nameMesh;
    std::string pathMesh;
    std::string nameDataset;
    std::string pathStatsManifold;
} OutputSweep;

typedef struct {
} OutputManifoldConfig;


typedef struct Config {
    VideoConfig videoConfig;
    SweepConfig sweepConfig;
    OutputSweep outputSweep;
} SweepConfiguration;

#endif /* TYPES_HPP_ */

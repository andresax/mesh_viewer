cmake_minimum_required(VERSION 2.8.3)

project(mesh_viewer)

#----------------------- CMAKE MODULES ------------------------

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules) 
 
# ----------------------- LIBRARIES ---------------------------

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(glfw REQUIRED) 
find_package(CGAL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ASSIMP REQUIRED) 
find_package(Eigen3 REQUIRED) 

  MESSAGE( STATUS "CMAKE_BUILD_TYPE:      " ${CMAKE_BUILD_TYPE} )
 
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  link_directories(external/TransforMesh/build)
  MESSAGE( STATUS "CMAKE_BUILD_TYPE:    release->     " ${CMAKE_BUILD_TYPE} )
# ----------------------- GCC FLAGS ----------------------------
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frounding-math  ")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories(external/TransforMesh/buildDebug)
  MESSAGE( STATUS "CMAKE_BUILD_TYPE:  debug->      " ${CMAKE_BUILD_TYPE} )
# ----------------------- GCC FLAGS ----------------------------
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frounding-math  ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frounding-math -fsanitize=address ")
endif()
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

# ----------------------- INCLUDES ----------------------------

include_directories(  ${OpenCV_INCLUDE_DIRS} 
                    ${EIGEN3_INCLUDE_DIR}  
                      ${OPENGL_INCLUDE_DIR} 
                      ${GLEW_INCLUDE_DIRS} 
                      ${GLFW_INCLUDE_DIR} 
                      external/glm/glm/
                      external/
                      external/TransforMesh/include
                      ${CGAL_INCLUDE_DIRS} 
                      ${ASSIMP_INCLUDE_DIRS}
                      include/mesh_viewer/)

# ----------------------- GCC FLAGS ----------------------------

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



# ------------------------ MESSAGES ----------------------------


# ------------------------ BUILD -------------------------------

file(GLOB_RECURSE mesh_viewer 
                  src/camera/*.cpp
                  src/OpenGLWrapper/*.cpp
                  src/*.cpp)

add_executable(mesh_viewer ${mesh_viewer} )
target_link_libraries(mesh_viewer ${OpenCV_LIBS}
                                  ${OPENGL_LIBRARIES}
                                  ${GLEW_LIBRARIES}
                                            glfw ${GLFW_LIBRARIES}
                                  TransforMesh
                                  ${CGAL_LIBRARY}
                                  ${Boost_LIBRARIES}
                                  ${ASSIMP_LIBRARIES}
                                  SOIL)


# ------------------------ INSTALLATION ------------------------


# --------------------------------------------------------------



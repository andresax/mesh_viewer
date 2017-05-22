#version 420

layout(location = 0) in vec3 position;  
layout(location = 1) in vec4 infoSurface;    

out vec3 positionPointV;
out vec4 projectorTexCoordV;  
out vec4 shadowCoord1V; 
out vec4 colorV;         

uniform mat4 MVP;

void main(){

  mat4 biasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
    );
  shadowCoord1V =  MVP * vec4(position, 1.0);
  shadowCoord1V = biasMatrix * shadowCoord1V;
  shadowCoord1V.z -= 0.000500;
  
  projectorTexCoordV = MVP * vec4(position, 1.0);
  projectorTexCoordV.y = -projectorTexCoordV.y;
  projectorTexCoordV = biasMatrix * projectorTexCoordV;

  positionPointV = position;
  gl_Position =  MVP * vec4(position,1.0);
  colorV = infoSurface;
}


#version 420

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;
 
in vec3 positionPointV[];       /*3D point position*/
in vec4 projectorTexCoordV[];  
in vec4 shadowCoord1V[];

out vec3 normal;
out vec4 projectorTexCoord;  
out vec4 shadowCoord1;         
out vec3 posV;

void main(){
  
  for(int i = 0; i < gl_in.length(); i++){

    normal = normalize(cross((positionPointV[2]).xyz - (positionPointV[0]).xyz, (positionPointV[1]).xyz - (positionPointV[0]).xyz));
    posV = positionPointV[i];
    projectorTexCoord = projectorTexCoordV[i];
    shadowCoord1 = shadowCoord1V[i];
    
    gl_Position = gl_in[i].gl_Position;

    EmitVertex();
  }
}
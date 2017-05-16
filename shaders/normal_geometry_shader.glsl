#version 420

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;
 
in vec4 positionPointV[];       /*3D point position*/

out vec3 normal;

void main(){
  
  for(int i = 0; i < gl_in.length(); i++){

    normal = normalize(cross((positionPointV[2]).xyz - (positionPointV[0]).xyz, (positionPointV[1]).xyz - (positionPointV[0]).xyz));
    
    EmitVertex();
  }
}
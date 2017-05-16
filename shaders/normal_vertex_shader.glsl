#version 420

in vec3 position;               /*3D vertex position*/

out vec3 positionPointV;        /*3D point position*/


void main(){


    gl_Position = vec4(position, 1.0);
    //3D position of the vertex
    positionPointV = position;

}


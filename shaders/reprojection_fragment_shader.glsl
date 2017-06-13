#version 420

in vec4 projectorTexCoord;
in vec4 shadowCoord1;
in vec3 normal;
in vec3 posV;
in vec4 color;

layout (location=0) out vec3 reproj;

uniform sampler2D image;
uniform vec3 camCenter;
uniform sampler2DShadow shadowMap1;

void main(){
  vec4 imageReproj = texture2DProj(image, projectorTexCoord);
  float shadowCoeff = textureProj(shadowMap1, shadowCoord1);


  vec3 l = normalize(posV);
  vec3 n = normalize(normal);
  float cosTheta = abs(dot(n, l));
  float cosTheta2 = abs(dot(-n, l));
  //illumination ambient + diffuse + specular
  vec3 ambient = vec3(0.05);
 // vec3 diffuse = color.xyz * max(cosTheta, cosTheta2);
  vec3 diffuse = vec3(1.0, 1.0, 1.0)* max(cosTheta, cosTheta2)*imageReproj.xyz;
  vec3 specular = vec3(0.0);

   
  reproj = shadowCoeff  * vec3(ambient+diffuse+specular ); 
   
  //reproj = shadowCoeff  * color.xyz; 
}

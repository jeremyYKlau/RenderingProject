#version 430 core

uniform mat4 modelview;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform float scaleX;
uniform float scaleY;
uniform float scaleZ;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

//attributes in camera coordinates
out vec3 N; //normal
out vec3 V; //view vector
out vec3 L; //light
out vec2 UV;

void main(void)
{
	UV = uv;


    vec4 lightCameraSpace = modelview * vec4(lightPosition, 1.0);
    lightCameraSpace /= lightCameraSpace.w;
	vec3 normal = normalize(-vertex);
    mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix * normal);

    float scale = 5.0;    
    vec3 positionModelSpace = vertex*scale + vec3(0.0, -0.42, 0.0);

    vec4 positionCameraSpace = modelview * vec4(positionModelSpace, 1.0);

    vec3 P = positionCameraSpace.xyz/positionCameraSpace.w;

    L = normalize(lightCameraSpace.xyz - P);
    V = normalize(-P);

    gl_Position = projection * positionCameraSpace;
}

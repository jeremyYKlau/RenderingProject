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

	//perhaps change these to input values so i can use mouse input to scale and rotate scene based off vertex shaders
	mat3 hemiRotate = mat3(
		1.0f, 0, 0, // first column (not row!)
		0, cos(-3.14/2), -sin(-3.14/2), // second column
		0, sin(-3.14/2), cos(-3.14/2)  // third column
	);

	mat3 hemiTranslate = mat3(
		scaleX, 1.0, 1.0,
		1.0, scaleY, 1.0,
		1.0, 1.0, scaleZ
	);

	mat3 hemiScale = mat3 (
		scaleX, 1.0, 1.0,
		1.0, scaleY, 1.0,
		1.0, 1.0, scaleZ
	);

    vec4 lightCameraSpace = modelview * vec4(lightPosition, 1.0);
    lightCameraSpace /= lightCameraSpace.w;
	vec3 normal = normalize(-vertex);
    mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix * normal);

    float scale =5.0;    
    vec3 positionModelSpace = vertex*scale + vec3(0.0, -0.42, 0.0);
    positionModelSpace = positionModelSpace*hemiRotate;

    vec4 positionCameraSpace = modelview * vec4(positionModelSpace, 1.0);

    vec3 P = positionCameraSpace.xyz/positionCameraSpace.w;

    L = normalize(lightCameraSpace.xyz - P);
    V = normalize(-P);

    gl_Position = projection * positionCameraSpace;
}

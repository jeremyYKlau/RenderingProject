#version 430 core

out vec4 color;

uniform sampler2D image;
uniform float pi;
in vec3 N;
in vec3 V;
in vec3 L;
in vec2 UV;

// Material properties
uniform vec3 diffuse_albedo = vec3(0.0,  1.0,  0.0); //blue
uniform vec3 specular_albedo = vec3(1.0); //white
uniform vec3 ambient_albedo = vec3(1.0);  //white


void main(void)
{    	

    // Calculate R locally this contains Rz,Ry,Rz
    vec3 R = reflect(-L, N);
    
    //the main chrome/reflection mapping algorithm, formula
    float theta = atan(R.x/R.y);
    float phi = acos(R.z);

	//normalize texture coordinates based off a sphere
	float u = theta/(2.f*pi);
	float v = phi/(pi);
    vec2 textureCoords = vec2(u,v);

	vec4 colorImage = texture(image, textureCoords);

    color = colorImage;
}

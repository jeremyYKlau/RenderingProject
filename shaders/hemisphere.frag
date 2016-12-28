#version 430 core

out vec4 color;

uniform sampler2D image;

in vec3 N;
in vec3 V;
in vec3 L;
in vec2 UV;


uniform float ka = 0.1;
uniform float kd = 0.5;
uniform float ks = 1.0;
uniform float ke = 5.0;

void main(void)
{
	// Calculate R locally
    vec3 R = reflect(-L, N);
    
    //The sphere mapping equation that allows the formation of the background on the sphere
    float m = 2*(pow((pow(R.x,2)+pow(R.y,2)+pow(R.z+1,2)),0.5));
	float u = (R.x/m)+(0.5);
	float v = (R.y/m)+(0.5);
	
	vec2 textureCoords = vec2(u,v);
	vec4 colorImage = texture(image, textureCoords);

    color = colorImage;
}



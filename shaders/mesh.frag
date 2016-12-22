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


uniform float ka = 0.1;
uniform float kd = 0.5;
uniform float ks = 1.0;
uniform float ke = 5.0;

void main(void)
{    	

    // Calculate R locally this contains Rz,Ry,Rz
    vec3 R = reflect(-L, N);
    
    //somewhere in here insert equation
    float theta = atan(R.y/R.x);
    float phi = acos(R.z);
    
    //normalizing the textureCoords created some very strange results
    vec2 textureCoords = vec2((theta/(2.f*pi)),(phi/(2*pi)));

	//colorImage is on it's own so we can actively change the way the final color comes out
	vec4 colorImage = texture(image, textureCoords);
            
    //these are probably irrelevant because i'm just going to use reflections mapping
    vec3 ambient =  ka * ambient_albedo;
    vec3 diffuse =  kd * max(dot(N, L), 0.0) * colorImage.xyz;
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * specular_albedo;

	//looks strange yet kinda cool color = vec4(diffuse, 1.0)*colorImage;
    color = colorImage;
}

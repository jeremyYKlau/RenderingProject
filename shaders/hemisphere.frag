#version 430 core

out vec4 color;

uniform sampler2D image;

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
	// Calculate R locally
    vec3 R = reflect(-L, N);
    /* This is indexing sphere maps
	float m = 2*pow((pow(R.x,2)+pow(R.y,2)+pow(R.z+1,2)),0.5);
	float u = (R.x/m)+(1/2);
	float v = (r.y/m)+(1/2);
	
	vec2 textureCoords = vec2(u,v);*/
	vec4 colorImage = texture(image, UV);

 

    vec3 ambient =  ka * ambient_albedo;
    vec3 diffuse =  kd * max(dot(N, L), 0.0) * colorImage.xyz;
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * specular_albedo;

    color = colorImage;
    // diffuse test
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}



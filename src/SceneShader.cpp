/*
 * SceneShader.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */
#include <iostream>

#include "SceneShader.h"
#include "lodepng.h"

static float PI = 3.14159265359;


using namespace std;

SceneShader::SceneShader(): Shader()
{
	_programPlane = 0;
	_programMesh = 0;
	_programHemisphere = 0;
	_planeVertexArray = -1;
	_planeVertexArray = -1;
	_mvUniform = -1;
	_projUniform = -1;
	_zTranslation = 1.5;
	_aspectRatio = 10.0;
	_xRot = 0.0;
	_yRot = 0.0;
	lX = 0.5;
	lY = 1.0;
	lZ = 0.5;
	lightPosition = glm::vec3(lX, lY, lZ);
	zoom = 45.0f;
	tMode = 1;
}

void SceneShader::calculateCylindricalUVCoordinates()
{
	for (unsigned int i = 0; i < _mesh->vertices.size(); i++)
	{
		glm::vec3 vertex(_mesh->vertices[i][0], _mesh->vertices[i][1], _mesh->vertices[i][2]);
		float theta = glm::atan(vertex.z, vertex.x)/PI;

		_uvs.push_back(glm::vec2(theta, vertex.y * 4.0));
	}
}

void SceneShader::readMesh( std::string filename )
{
	_mesh = trimesh::TriMesh::read(filename);

	_mesh->need_bbox();
	_mesh->need_faces();
	_mesh->need_normals();
	_mesh->need_bsphere();

	for(unsigned int i = 0; i < _mesh->faces.size(); i++)
	{
   	_triangleIndices.push_back(_mesh->faces[i][0]);
	  _triangleIndices.push_back(_mesh->faces[i][1]);
	  _triangleIndices.push_back(_mesh->faces[i][2]);
	}

	calculateCylindricalUVCoordinates();
}

//the function for loading the textures
//takes user input to change textures
void SceneShader::loadTextures()
{
	glDeleteTextures(1, &_textureID);
	std::string imageFilename;
	if (tMode == 1)
		{
			imageFilename = std::string("textures/brick.png");
		}
	else if (tMode == 2)
		{
			imageFilename = std::string("textures/chrome.png");
		}
	else if (tMode == 3)
		{
			imageFilename = std::string("textures/texture1.png");
		}
	else if (tMode == 4)
		{
			imageFilename = std::string("textures/lion.png");
		}
	else if (tMode == 5)
		{
			imageFilename = std::string("textures/plain.png");
		}
	else if (tMode == 6)
		{
			imageFilename = std::string("textures/interior.png");
		}
	//initialize an empty vector for image because it is neccessary or else it won't switch textures
	std::vector<unsigned char> _image;
	
	unsigned int error = lodepng::decode(_image, _imageWidth, _imageHeight, imageFilename.c_str());
	if (error)
	{
		std::cout << "reading error" << error << ":" << lodepng_error_text(error) << std::endl;
	}

	_textureImageID = _texture.create2DTexture(_image, _imageWidth, _imageHeight);
	cout << "loadingTextures" << endl;
}

//create exterior sphere
void SceneShader::createHemisphere()
{
	//interval is for how much space to partition each angle when generating new triangle
	int interval = 5;
	//r is the radius of sphere
	float r = 1;
	//interval but calculated in sphereical coordinates
	float t;
	float x = 0;
	float y = 0;
	float z = 0;
	float u;
	float v;
	/*
	using phi and theta create intervals to create a single 4 point quad to render as part of the hemisphere
	these 4 points are stored in an array of vec3's
	texture coordinates are stored in an array of vec2's
	push the vectors stored at each point for texture and position to be rendered in shaders
	*/
	for (int phi = 0; phi <= 180; phi = phi + interval)
	{
		for (int theta = 0; theta <= 360; theta = theta + interval)
		{
			t = interval*(PI/180.f);
			//Point 1
			x = r*(cos(theta*(PI/180.f))*sin(phi*(PI/180.f)));
			y = r*(sin(theta*(PI/180.f))*sin(phi*(PI/180.f)));
			z = r*cos(phi*(PI/180.f));
			glm::vec3 p0 = glm::vec3(x,y,z);
			ePos.push_back(p0);
			//normalize texture coordinates so that it accesses all space from the texture given
			u = (theta)/(2.0*PI);
			v = (phi)/(PI);
			glm::vec2 t0 = glm::vec2(u,v);
			eTex.push_back(t0);

			//Point 2
			x = r*(cos(theta*(PI/180.f)+t)*sin(phi*(PI/180.f)));
			y = r*(sin(theta*(PI/180.f)+t)*sin(phi*(PI/180.f)));
			z = r*cos(phi*(PI/180.f));
			glm::vec3 p1 = glm::vec3(x,y,z);
			ePos.push_back(p1);
			//normalize texture coordinates so that it accesses all space from the texture given
			u = ((theta)/(2.0*PI))+(t);
			v = ((phi)/(PI));
			glm::vec2 t1 = glm::vec2(u,v);
			eTex.push_back(t1);

			//Point 3
			x = r*(cos(theta*(PI/180.f))*sin(phi*(PI/180.f)+t));
			y = r*(sin(theta*(PI/180.f))*sin(phi*(PI/180.f)+t));
			z = r*cos(phi*(PI/180.f)+t);
			glm::vec3 p2 = glm::vec3(x,y,z);
			ePos.push_back(p2);
			//normalize texture coordinates so that it accesses all space from the texture given
			u = ((theta)/(2.0*PI));
			v = ((phi)/(PI))+(t);
			glm::vec2 t2 = glm::vec2(u,v);
			eTex.push_back(t2);

			//Point 4
			x = r*(cos(theta*(PI/180.f)+t)*sin(phi*(PI/180.f)+t));
			y = r*(sin(theta*(PI/180.f)+t)*sin(phi*(PI/180.f)+t));
			z = r*cos(phi*(PI/180.f)+t);
			glm::vec3 p3 = glm::vec3(x,y,z);
			ePos.push_back(p3);
			//normalize texture coordinates so that it accesses all space from the texture given
			u = ((theta)/(2.0*PI))+(t);
			v = ((phi)/(PI))+(t);
			glm::vec2 t3 = glm::vec2(u,v);
			eTex.push_back(t3);
		}
	}
}
//create the buffers for rendering of the scene
void SceneShader::createVertexBuffer()
{

	createHemisphere();
	std::vector<GLubyte> colormap;
	//red
	colormap.push_back(255);
	colormap.push_back(0);
	colormap.push_back(0);
	colormap.push_back(255);

	//blue
	colormap.push_back(0);
	colormap.push_back(0);
	colormap.push_back(255);
	colormap.push_back(255);

	//creating texture
	_textureID = _texture.create1DTexture(colormap);

	//create plane geometry
	static const GLfloat quadData[] =
	{
		-10.0f, -1.0f, -100.0f,
		-10.0f, -1.0f,  100.0f,
		 10.0f, -1.0f, -100.0f,
		 10.0f, -1.0f,  100.0f,
	};

	//create plane UV coordinates
	static const GLfloat uvsPlane[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	//creating texture 2D
	//calls function that switches textures from the ones I have already chosen
	loadTextures();

	//passing model attributes to the GPU
	//plane
	glGenVertexArrays(1, &_planeVertexArray);
	glBindVertexArray(_planeVertexArray);

	glGenBuffers(1, &_planeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _planeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//uvsPlane buffer
	glGenBuffers(1, &_planeTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _planeTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (uvsPlane), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//read and create mesh geometry
	readMesh("./models/cube.ply");
	
	//triangle mesh
	glGenVertexArrays(1, &_meshVertexArray);
	glBindVertexArray(_meshVertexArray);

	glGenBuffers(1, &_meshVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _mesh->vertices.size() * sizeof (trimesh::point), _mesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, _mesh->normals.size() * sizeof(trimesh::vec), _mesh->normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	//mesh UVs buffer
	glGenBuffers(1, &_meshTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), _uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &_meshIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices.size()*sizeof(unsigned int), _triangleIndices.data(), GL_STATIC_DRAW);


	//triangle mesh for the hemisphere
	glGenVertexArrays(1, &_hemisphereVertexArray);
	glBindVertexArray(_hemisphereVertexArray);

	glGenBuffers(1, &_hemisphereVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _hemisphereVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  ePos.size() * sizeof (glm::vec3), ePos.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//NOTE the hemisphere does not have normals stored like the Trimesh mesh

	//for hemisphere UVs
	glGenBuffers(1, &_hemisphereTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _hemisphereTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, eTex.size() * sizeof(glm::vec2), eTex.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	
	glBindVertexArray(0);

}


void SceneShader::startup()
{
	_programPlane = compile_shaders("./shaders/plane.vert", "./shaders/plane.frag");

	_programMesh = compile_shaders("./shaders/mesh.vert", "./shaders/mesh.frag");

	_programLight = compile_shaders("./shaders/light.vert", "./shaders/light.frag");

	_programHemisphere = compile_shaders("./shaders/hemisphere.vert", "./shaders/hemisphere.frag");

	createVertexBuffer();

}
//code if a plane wants to be added in the future, in my code the plane doesn't work for some reason
void SceneShader::renderPlane()
{
	glBindVertexArray(_planeVertexArray);

	glUseProgram(_programPlane);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//activating texture
	_texture.bind2DTexture(_programPlane, _textureImageID, std::string("image"));

	//scene matrices and camera setup
	glm::vec3 eye(0.0f,0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	glm::mat4 identity(1.0f);
	_projection = glm::perspective( zoom, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(identity, _xRot  * PI/180.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	_modelview *=  rotationX;
	_modelview *=  rotationY;

	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programPlane, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glUniform1f(glGetUniformLocation(_programMesh, "pi"), PI);
	
	glDrawArrays(GL_TRIANGLES, 0, 4);

	_texture.unbind2DTexture();

	glDisable(GL_BLEND);

	glBindVertexArray(0);
}

//renders interior object
void SceneShader::renderMesh()
{
	glBindVertexArray(_meshVertexArray);

	glUseProgram(_programMesh);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_texture.bind2DTexture(_programMesh, _textureImageID, std::string("image"));

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( zoom, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(identity, _xRot  * PI/180.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        _modelview *=  rotationX;
		_modelview *=  rotationY;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programMesh, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glUniform1f(glGetUniformLocation(_programMesh, "pi"), PI);

	glDrawElements( GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0 );

	glBindVertexArray(0);

	_texture.unbind2DTexture();
}

//renders exterior sphere
void SceneShader::renderHemisphere()
{
	glBindVertexArray(_hemisphereVertexArray);

	glUseProgram(_programHemisphere);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_texture.bind2DTexture(_programHemisphere, _textureImageID, std::string("image"));

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( zoom, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(identity, _xRot  * PI/180.0f, glm::vec3(5.0f, 0.0f, 0.0f));

	_modelview *=  rotationX;
	_modelview *=  rotationY;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programHemisphere, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programHemisphere, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programHemisphere, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glUniform1f(glGetUniformLocation(_programMesh, "pi"), PI);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(glm::vec3)*ePos.size());

	glBindVertexArray(0);

	_texture.unbind2DTexture();
}

//the light as just a point
void SceneShader::renderLight()
{
	glUseProgram(_programLight);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( zoom, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(identity, _xRot  * PI/180.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        _modelview *=  rotationX;
		_modelview *=  rotationY;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glPointSize(30.0f);
	glDrawArrays( GL_POINTS, 0, 1);

}

void SceneShader::render()
{
	renderMesh();
	renderLight();
	renderHemisphere();
}

void SceneShader::setZTranslation(float z)
{
	zoom = z * -_zTranslation;
}

void SceneShader::setAspectRatio(float ratio)
{
	_aspectRatio = ratio;
}

void SceneShader::setRotationX( float x )
{
	_xRot = x;
}

void SceneShader::setRotationY( float y )
{
	_yRot = y;
}

void SceneShader::shutdown()
{
//delete center object
	glDeleteBuffers(1, &_meshVertexBuffer);
	glDeleteBuffers(1, &_meshNormalBuffer);
	glDeleteBuffers(1, &_meshIndicesBuffer );
	glDeleteVertexArrays(1, &_meshVertexArray);

//delete plane vertices
	glDeleteVertexArrays(1, &_planeVertexArray);

//delete hemisphere
	glDeleteBuffers(1, &_hemisphereVertexBuffer);
	glDeleteBuffers(1, &_hemisphereNormalBuffer);
	glDeleteBuffers(1, &_hemisphereIndicesBuffer);
	glDeleteVertexArrays(1, &_hemisphereVertexArray);
}

void SceneShader::updateLightPositionX(float x)
{
	lightPosition.x += x;
}

void SceneShader::updateLightPositionY(float y)
{
	lightPosition.y += y;
}

void SceneShader::updateLightPositionZ(float z)
{
	lightPosition.z += z;
}

SceneShader::~SceneShader()
{
	shutdown();
}

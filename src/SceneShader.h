/*
 * SceneShader.h
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */

#ifndef SCENESHADER_H_
#define SCENESHADER_H_

#include "Shader.h"
#include <vector>

#define GLM_FORCE_RADIANS

#include "TriMesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

class SceneShader : public Shader
{
public:

	SceneShader();
	~SceneShader();

	void startup ();
	void shutdown ();
	void render();

	void renderPlane();
	void renderMesh();
	void renderLight();
	void renderHemisphere();

	void setAspectRatio( float ratio );
	void setZTranslation(float z);
	void setRotationX( float x );
	void setRotationY( float y );

	void updateLightPositionX( float x );
	void updateLightPositionY( float y );
	void updateLightPositionZ( float z );
	void loadTextures();

	//value for changing the textures
	int tMode;
	//value for changing the model
	int mMode;
	float zoom;
	
private:

	/*methods*/
	void calculateCylindricalUVCoordinates();
	void calculateCylindricalUVCoordinatesHemi();

	void readMesh( std::string filename );
	void createVertexBuffer();
	void createHemisphere();

	//image
	std::vector<unsigned char> _image;
	unsigned int _imageWidth;
	unsigned int _imageHeight;

	//plane texture image

	/*variables*/
	GLuint _programLight;
	GLuint _programPlane;
	GLuint _programMesh;
	GLuint _programHemisphere;


	GLuint _planeVertexArray;
	GLuint _planeVertexBuffer;
	GLuint _planeTextureBuffer;

	GLuint _meshVertexArray;
	GLuint _meshVertexBuffer;
	GLuint _meshNormalBuffer;
	GLuint _meshTextureBuffer;
	GLuint _meshIndicesBuffer;

	GLuint _hemisphereVertexArray;
	GLuint _hemisphereVertexBuffer;
	GLuint _hemisphereNormalBuffer;
	GLuint _hemisphereTextureBuffer;
	GLuint _hemisphereIndicesBuffer;

  GLint _mvUniform, _projUniform;

	/* Matrices */
	glm::mat4 _modelview;
	glm::mat4 _projection;

	float _zTranslation;
	float _xRot;
	float _yRot;
	float _aspectRatio;
	float initialPerspective;

	/* Textures */
	GLuint _textureID;
	GLuint _textureImageID;
	Texture _texture;
	std::vector<glm::vec2> _uvs;
	std::vector<glm::vec2> _uvsHemisphere;
	
	//tecture and position vectors for hemisphere
	std::vector<glm::vec3> ePos;
	std::vector<glm::vec2> eTex;
	
	/*vertices*/
	std::vector<float> _vertices;

	trimesh::TriMesh* _mesh;

	std::vector<unsigned int> _triangleIndices;

	glm::vec3 lightPosition;

};

#endif /* SCENESHADER_H_ */

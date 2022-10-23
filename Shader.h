#pragma once
#define _USE_MATH_DEFINES
#include <algorithm>
#include <iostream>
#include <list>
#include <cmath>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>





using namespace glm;
using namespace std;


class Shader {
public:
	//Normal Shaders for rendering Colors
	const char* getVertexShader();

	//Color Fragment Shader
	const char* getFragmentShader();
	
	//Shadow Shaders
	const char* getVertexShaderShadow();
	const char* getFragmentShaderShadow();
	const char* getGeometryShaderShadow();
	//SkyCube Shaders
	const char* getVertexShaderCubeMap();
	const char* getFragmentShaderCubeMap();
	const char* getVertexShaderSky();
	const char* getFragmentShaderSky();

	//Particule Shaders
	const char* getVertexShaderParticles();
	const char* getFragmentShaderParticles();
	//Text Shader
	const char* getVertexShaderText();
	const char* getFragmentShaderText();
	//Compile any type of Shaders
	int compileAndLinkShaders(const char* vertex, const char* fragment, const char* geometry=nullptr);
	
	//set the TextureProgram
	void setShaderProgram(int shaderTextureProgram,vec3 lightPosition,vec3 cameraPosition,mat4 viewMatrix);
	//create depth cubemap texture
	void createDepthCubeMap(int SHADOW_WIDTH,int SHADOW_HEIGHT);
	void setShadow(int shaderShadowProgram, vec3 lightPos, int SHADOW_WIDTH, int SHADOW_HEIGHT);
	int createVertexArraySkyBox();
	int createVertexArrayObject();
	GLuint setupModelEBO(string path, int& vertexCount);
	bool initContext();
	GLFWwindow* window = NULL;

	unsigned int getDepthCubeMap();
	float getFar_plane();

private:


	//shadow attributes
	unsigned int depthMapFBO;
	float far_plane;
	unsigned int depthCubeMap;
	

};

#pragma once

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif  // __APPLE__

#include <string>
#include <vector>

#include "Camera.h"
#include "GLSLShader.h"
#include "Resources.h"
#include "MyHelpers.h"

#include <iostream>

using namespace std;


class Display {

public:
	Display(int w, int h, const char* title);
	~Display();

	void Run();

	Mat4 projection;
	Mat4 view;
	Mat4 PMVmatrix;

	Camera MainCam;
	double MouseLastX = 0;
	double MouseLastY = 0;

private:

	void init();
	void displayLoop();
	void renderObj(GLSLShader shader, string objPath, vector<string> textureFiles, vector<string> textureLocs);
	void updateCam();

	int _windowHeight;
	int _windowWidth;


	GLFWwindow* _window;
	Resources _resManager;

	GLSLShader _defaultShader;
	GLSLShader _terrainShader;
	GLSLShader _heightmapShader;

	vector<int> _rawIds;

	Vec4 LightPosition = Vec4( 0, 0, 0, 1 );
	Vec4 SkyAmbience = Vec4( -0.7f, -0.7f, -0.6f, 1.0f );
	Vec4 Ambient = Vec4( 0.2f, 0.2f, 0.2f, 1.0f);
	Vec4 Diffuse = Vec4( 0.8f, 0.8f, 0.8f, 1.0f );
	Vec4 Specular = Vec4( 0.01f, 0.01f, 0.01f, 1.0f );
	GLfloat shininess = 0.2f;
};

#pragma once

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif  // __APPLE__

#include <stdio.h>
#include <stdlib.h>
#include "Resources.h"

class GLSLShader {

public:

	GLSLShader(const char* vertexShaderFile, const char* fragmentShaderFile);
	GLSLShader();
	~GLSLShader();

	void Use();

	GLuint GetUniformLocation(const char* loc);
	GLuint GetAttribLocation(const char* loc);
	GLuint GetId();

	GLuint GetMV();
	GLuint GetProj();

	const char* GetPosition() { return "vPos"; };
	const char* GetNorm() { return "vNorm"; };
	const char* GetTexture() { return "vTex"; };

protected:

	struct Shader {
		const char*  filename;
		GLenum type;
		unsigned char* source;
	};

	GLuint _program;
	GLuint _mv;
	GLuint _proj;
	GLuint initShader(const char* vertexShaderFile, const char* fragmentShaderFile);
};
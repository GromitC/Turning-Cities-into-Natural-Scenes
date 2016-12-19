#pragma once

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif  // __APPLE__

#include "GLObjects.h"
#include "picoPNG.h"
#include "obj_parser.h"


#include "tga.h"

#include <map>
#include <string>
#include <iostream>
using namespace std;

// Forward declaration
class GLSLShader;

class Resources {

public:

	Resources();
	~Resources();

	GLTexture GetTexture(std::string texturePath);
	GLObj GetObject(std::string objPath, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs,int mode, bool forceReload);

	bool ScaleObject(std::string objectPath, Vec3 scale);
	bool TranslateObject(std::string objectPath, Vec3 trans);


	GLuint CreateVAO();
	void UnbindVAO();

	GLuint MakeVAO(GLSLShader* shader, float* vp, float* vn, float* vt, int size, bool tex);

	bool ParseTerrain(std::string heightmap,float*& _vertices,float*& _textures,float*& _normals,int& point_count);

    static unsigned char* ReadFileSource(const char* shaderFile, long &size);


private:
	
	void loadVAO(int loc, int size, float* data);

    std::map<std::string, GLTexture> _textureCache;
    std::map<std::string, GLObj> _objectCache;

    GLuint active_vbo;


    GLTexture loadPNG(std::string filename);
    GLObj loadObj(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
    GLObj loadTerrain(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
};
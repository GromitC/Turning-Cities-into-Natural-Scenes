#pragma once

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#endif  // __APPLE__

#include "MyHelpers.h"

#include <string>
#include <vector>

struct GLTexture {
        std::string filename;
        unsigned int GLid;
        int width;
        int height;
};

struct GLObj {
        std::string filename;
        unsigned int vaoId;
        int size;
        GLuint shader;
        std::vector<GLuint> textures;
        std::vector<GLuint> textureLocs;
        Vec3 Position = Vec3(0, 0, 0);
        Vec4 Rotation = Vec4(0, 1, 0, 0);
        Vec3 Scale = Vec3(1, 1, 1);
};

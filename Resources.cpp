#include "Resources.h"

// To resolve forward declaration
#include "GLSLShader.h"


Resources::Resources() {

}

Resources::~Resources() {

}

// GLTexture Resources::GetTexture(std::string texturePath) {

//     auto text = _textureCache.find(texturePath);

//     // Load and insert if not in cache
//     if (text == _textureCache.end()) {
//         cout<<"load texture"<<endl;
//         GLTexture newTexture = loadPNG(texturePath);
//         _textureCache.insert(std::make_pair(texturePath, newTexture));

//         return newTexture;
//     }

//     return text->second;
// }

GLObj Resources::GetObject(std::string objectPath, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs,int mode,bool forceReload) {

    auto object = _objectCache.find(objectPath);
    GLObj newObject;
    // Load and insert if not in cache
    if (object == _objectCache.end() || forceReload) {
        if(forceReload){
            for (auto texture: object->second.textures){
                glDeleteTextures(1, &texture );
            }
        }
        if(mode==1){

           newObject = loadObj(objectPath, shader, true, textureFiles, textureLocs);
        }
        else{
           newObject = loadTerrain(objectPath, shader, tex, textureFiles, textureLocs);
        }
        
        //GLObj newObject = loadObj(objectPath, shader, false, textureFiles, textureLocs);
        _objectCache.insert(std::make_pair(objectPath, newObject));

        return newObject;
    }

    return object->second;
}

bool Resources::ScaleObject(std::string objectPath, Vec3 scale) {
    auto object = _objectCache.find(objectPath);

    if (object != _objectCache.end()) {
        GLObj o = object->second;
        o.Scale = scale;
        object->second = o;

        return true;
    }

    return false;
}

bool Resources::TranslateObject(std::string objectPath, Vec3 trans) {
    auto object = _objectCache.find(objectPath);

    if (object != _objectCache.end()) {
        GLObj o = object->second;
        o.Position = trans;
        object->second = o;

        return true;
    }

    return false;
}


GLuint Resources::CreateVAO() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

void Resources::UnbindVAO() {
    glBindVertexArray(0);
}

GLuint Resources::MakeVAO(GLSLShader* shader, float* vp, float* vn, float* vt, int size, bool tex) {
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * size, vp, GL_STATIC_DRAW);

    GLuint vbo_norm;
    glGenBuffers (1, &vbo_norm);
    glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 3 * size, vn, GL_STATIC_DRAW);

    GLuint vbo_tex;
    if (tex)
    {
        glGenBuffers (1, &vbo_tex);
        glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
        glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * 2 * size, vt, GL_STATIC_DRAW);

        // for (int i = 0; i < textureFiles.size(); i++) {
        //     GLTexture texture = loadPNG(textureFiles[i]);
        //     obj.textures.push_back(texture.GLid);
        //     obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
        // }
    }

    GLuint vao = CreateVAO();
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    GLuint pos = glGetAttribLocation(shader->GetId(), shader->GetPosition());
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    glBindBuffer (GL_ARRAY_BUFFER, vbo_norm);
    pos = glGetAttribLocation(shader->GetId(), shader->GetNorm());
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    if (tex)
    {
        glBindBuffer (GL_ARRAY_BUFFER, vbo_tex);
        pos = glGetAttribLocation(shader->GetId(), shader->GetTexture());
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);   
    }

    return vao;
}

void Resources::loadVAO(int loc, int size, float* data) {
	GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * size, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLTexture Resources::loadPNG(std::string filename) {
    cout<<"load "<<filename<<endl;
    GLTexture texture = {};

    unsigned long width, height;

    long in_size;
    unsigned char* in = Resources::ReadFileSource(filename.c_str(), in_size);
    std::vector<unsigned char> out;

    if (in == NULL) {
        fprintf(stderr, "Failed to load PNG file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    int err = decodePNG(out, width, height, in, in_size);
    if (err != 0) {
        fprintf(stderr, "decodePNG %s failed with error:  %d!\n", filename.c_str(),err);
		exit(EXIT_FAILURE);
    }

    glGenTextures(1, &(texture.GLid));

    glBindTexture(GL_TEXTURE_2D, texture.GLid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = width;
    texture.height = height;
    texture.filename = filename;

    return texture;
}

// Modified from:
// https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/21_cube_mapping/main.cpp
GLObj Resources::loadObj(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs) {
	GLObj obj = {};

    GLfloat* vp = NULL; // array of vertex points
    GLfloat* vn = NULL; // array of vertex normals
    GLfloat* vt = NULL; // array of texture coordinates
    int g_point_count = 0;
    if (!load_obj_file (filename.c_str(), vp, vt, vn, g_point_count))
    {
        fprintf(stderr, "Failed to load OBJ file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    if (vp == NULL || vn == NULL)
    {
        fprintf(stderr, "Failed to load OBJ file to buffer %s!\n", filename.c_str());
		exit(EXIT_FAILURE);
    }

    if (vp != NULL)
        free(vp);
    if (vn != NULL)
        free(vn);
    if (vt != NULL)
        free(vt);

    obj.size = g_point_count;
    obj.vaoId = MakeVAO(shader, vp, vn, vt, g_point_count, tex);//, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
    obj.filename = filename;
    obj.shader = shader->GetId();

    for (int i = 0; i < textureFiles.size(); i++) {
        GLTexture texture = loadPNG(textureFiles[i]);
        obj.textures.push_back(texture.GLid);
        obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
    }

    UnbindVAO();

    return obj;
}

GLObj Resources::loadTerrain(std::string filename, GLSLShader* shader, bool tex, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs) {
    GLObj obj = {};

    GLfloat* vp = NULL; // array of vertex points
    GLfloat* vn = NULL; // array of vertex normals
    GLfloat* vt = NULL; // array of texture coordinates
    int g_point_count = 0;
    ParseTerrain(filename.c_str(), vp, vt, vn, g_point_count);

    if (vp == NULL || vn == NULL)
    {
        fprintf(stderr, "Failed to load Terrain file to buffer %s!\n", filename.c_str());
        exit(EXIT_FAILURE);
    }

    if (vp != NULL)
        free(vp);
    if (vn != NULL)
        free(vn);
    if (vt != NULL)
        free(vt);

    obj.size = g_point_count;
    obj.vaoId = MakeVAO(shader, vp, vn, vt, g_point_count, tex);//, std::vector<std::string> textureFiles, std::vector<std::string> textureLocs);
    obj.filename = filename;
    obj.shader = shader->GetId();

    for (int i = 0; i < textureFiles.size(); i++) {
        GLTexture texture = loadPNG(textureFiles[i]);
        obj.textures.push_back(texture.GLid);
        obj.textureLocs.push_back(glGetUniformLocation(shader->GetId(), textureLocs[i].c_str()));
    }

    UnbindVAO();

    return obj;
}

unsigned char* Resources::ReadFileSource(const char* shaderFile, long &size) {
	FILE* fp = fopen(shaderFile, "r");
	size = 0;

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	unsigned char* buf = (unsigned char *) malloc(size + 1);
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

bool Resources::ParseTerrain(std::string heightmap,float*& _vertices,float*& _textures,float*& _normals,int& point_count) {
    unsigned long _length,_width;
    float* _heights;
    long in_size;
    unsigned char* in = ReadFileSource(heightmap.c_str(), in_size);
    std::vector<unsigned char> out;

    if (in == NULL) {
        fprintf(stderr, "Failed to load PNG file to buffer %s!\n", heightmap.c_str());
        exit(EXIT_FAILURE);
    }

    int err = decodePNG(out, _width, _length, in, in_size);
    if (err != 0) {
        fprintf(stderr, "decodePNG failed with error:  %d!\n", err);
        exit(EXIT_FAILURE);
    }

    _heights = new float[_width * _length*2];
    _normals = new float[_width * _length*2 * 3];
    _vertices = new float[_width * _length*2 * 3];
    _textures = new float[_width * _length*2 * 2];

    //point_count=_width * _length*2;


    printf("Length: %lu\n", out.size());
    printf("Height Width: %lu x %lu\n", _length, _width);

    int dummy=0;
     for ( unsigned int i = 0; i < _width; ++i )
    {
        for ( unsigned j = 0; j < _length; ++j )
        {
            dummy++;
            unsigned int index = ( i * _width ) + j;
            float heightValue = out[index * 4] / 256.0;
 
            float X = (i / (float)(_width ) )-0.5f;
            float Z = ( j / (float)(_length) )-0.5f;

            float Y = heightValue;// * m_fHeightScale;
            _normals[index * 3] = 0;
            _normals[index * 3 + 1] = 1;
            _normals[index * 3 + 2] = 0;

            _vertices[index * 3] = X;
            _vertices[index * 3 + 1] = Y;
            _vertices[index * 3 + 2] = Z;

            _textures[index * 2] = X;
            _textures[index * 2 + 1] = Z;
        }
    }
    //dummy++;

    for ( unsigned int j = 0; j < _length; ++j )
    {
        for ( unsigned i = 0; i < _width; ++i )
        {

            dummy++;
            unsigned int index = dummy;
            unsigned int _index = ( i * _width ) + j;
            if (_index>=_width*_length-1){
                break;
            }
            float heightValue = out[_index * 4] / 256.0;
 
            float X = (i / (float)(_width ) )-0.5f;
            float Z = ( j / (float)(_length) )-0.5f;

            float Y = heightValue;

            _normals[index * 3] = 0;
            _normals[index * 3 + 1] = 1;
            _normals[index * 3 + 2] = 0;

            _vertices[index * 3] = X;
            _vertices[index * 3 + 1] = Y;
            _vertices[index * 3 + 2] = Z;

            _textures[index * 2] = X;
            _textures[index * 2 + 1] = Z;
            //cout<<"normal"<<endl;

        }
    }
    point_count=dummy;
    return true;

} 
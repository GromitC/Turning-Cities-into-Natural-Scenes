#include "GLSLShader.h"

GLSLShader::GLSLShader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	_program = initShader(vertexShaderFile, fragmentShaderFile);

	_mv = glGetUniformLocation(_program, "ModelView");
	_proj = glGetUniformLocation(_program, "Projection");
}

GLSLShader::GLSLShader() {

}

GLSLShader::~GLSLShader() {

}

void GLSLShader::Use() {
	glUseProgram(_program);
}

GLuint GLSLShader::GetId() {
	return _program;
}

GLuint GLSLShader::GetUniformLocation(const char* loc) {
	return glGetUniformLocation(_program, loc);
}

GLuint GLSLShader::GetAttribLocation(const char* loc) {
	return glGetAttribLocation(_program, loc);
}

GLuint GLSLShader::GetMV() {
	return _mv;
}

GLuint GLSLShader::GetProj() {
	return _proj;
}

GLuint GLSLShader::initShader(const char* vertexShaderFile, const char* fragmentShaderFile) {

	Shader shaders[2] = {
		{ vertexShaderFile, GL_VERTEX_SHADER, NULL },
		{ fragmentShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();
	
	int i;
	struct Shader *s;
	GLint shader, compiled, linked, logSize;
	char *logMsg;
	long fSize;
	
	for (i = 0; i < 2; ++i) {
		
		s = &shaders[i];		
		s->source = Resources::ReadFileSource(s->filename, fSize);
		if (shaders[i].source == NULL) {
			fprintf(stderr, "Failed to read %s\n", s->filename);
			exit(EXIT_FAILURE);
		}

		shader = glCreateShader(s->type);
		glShaderSource(shader, 1, (const GLchar**) &s->source, NULL);
		glCompileShader(shader );

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			fprintf(stderr, "%s failed to compile\n", s->filename);
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			logMsg = (char *) malloc(logSize);
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			fprintf(stderr, "%s\n", logMsg);
			free(logMsg);

			exit(EXIT_FAILURE);
		}

		free(s->source);

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		fprintf(stderr, "Shader program failed to link\n");
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		logMsg = (char *) malloc(logSize);
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		fprintf(stderr, "%s\n", logMsg);
		free(logMsg);

		exit( EXIT_FAILURE );
	}

	return program;
}

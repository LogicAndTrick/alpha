#include <iostream>
#include <fstream>
#include <string>

#include "shader.h"

using namespace std;

GLuint shaderLoadFromFile(const char* filename, GLenum shaderType) {
	ifstream file(filename);
	if (!file) {
		cerr << "Unable to open file: " << filename << endl;
		return 0;
	}

	char line[256];
	string source;

	while (file) {
		file.getline(line, 256);
		source += line;
		source += '\n';
	}

    if (!file.eof()) {
    	cerr << "Error reading the file: " << filename << endl;
    	return 0;
    }

    GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		cerr << "Failed to compile shader: " << filename << endl;
		GLchar log[10000];
		glGetShaderInfoLog(shader, 10000, NULL, log);
		cerr << log << endl;
		exit(1);
	}

	return shader;
}
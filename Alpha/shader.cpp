#include <iostream>
#include <fstream>
#include <string>

#include "shader.h"

using namespace std;

namespace shader {

    GLuint LoadFromFile(const char* filename, GLenum shaderType) {
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
        const char *cstr = source.c_str();
	    glShaderSource(shader, 1, (const GLchar**)&cstr, NULL);
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


    GLuint CreateProgram(GLuint vert, GLuint frag) {
	    GLint status;
	    GLuint prog = glCreateProgram();

	    glAttachShader(prog, vert);
	    glAttachShader(prog, frag);

	    glLinkProgram(prog);
	    glGetProgramiv(prog, GL_LINK_STATUS, &status);
	    if (status != GL_TRUE) {
		    cerr << "Failed to link shaders: " << endl;
		    GLchar log[10000];
		    glGetProgramInfoLog(prog, 10000, NULL, log);
		    cerr << log << endl;
	    }

	    return prog;
    }
}
#include <iostream>
#include <fstream>
#include <string>

#include "globals.h"
#include "shader.h"

using namespace std;

namespace shader {

    program LoadProgramFromFiles(const char* vertFileName, const char* fragFileName)
    {
        program prog;
        char path[260] = "";

        globals::ResolvePath(vertFileName, path);
        prog.vertShader = shader::LoadFromFile(path, GL_VERTEX_SHADER);

        globals::ResolvePath(fragFileName, path);
        prog.fragShader = shader::LoadFromFile(path, GL_FRAGMENT_SHADER);

        prog.id = CreateProgram(prog.vertShader, prog.fragShader);
        return prog;
    }

    void DestroyProgram(program prog)
    {
        glDeleteProgram(prog.id);
        glDeleteShader(prog.vertShader);
        glDeleteShader(prog.fragShader);
    }
    
    void Bind(program prog)
    {
        glUseProgram(prog.id);
    }

    void Unbind()
    {
        glUseProgram(0);
    }

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
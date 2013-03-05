#pragma once

#include "gl/glew.h"


namespace shader {

    GLuint LoadFromFile(const char* filename, GLenum shaderType);

    GLuint CreateProgram(GLuint vert, GLuint frag);

}
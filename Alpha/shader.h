#pragma once

#include "gl/glew.h"


namespace shader {

    typedef struct {
        GLuint vertShader;
        GLuint fragShader;
        GLuint id;
    } program;

    typedef struct {
        GLuint buffer;
        GLuint texture;
    } framebuffer;

    program LoadProgramFromFiles(const char* vertFileName, const char* fragFileName);
    void DestroyProgram(program prog);
    
    void Bind(program prog);
    void Unbind();

    GLuint LoadFromFile(const char* filename, GLenum shaderType);

    GLuint CreateProgram(GLuint vert, GLuint frag);

}
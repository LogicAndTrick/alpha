#pragma once

#include <list>
#include "glm/glm.hpp"
#include "gl/glew.h"
#include "shader.h"
#include "gamemode.h"

class LineMode : public GameMode
{
public:
    LineMode(void);
    ~LineMode(void);

    void Initialise();
    void Destroy();

    void Update();
    void Render();

    void OnMouseMove(int x, int y, int deltaX, int deltaY);
    void OnResize(int w, int h);
private:
    std::list<glm::vec3> *points;
    static const int MAX_POINTS = 50;
    
    GLuint lineVertShader;
    GLuint lineFragShader;
    GLuint passthroughVertShader;
    GLuint passthroughFragShader;
    GLuint gaussianVFragShader;
    GLuint gaussianHFragShader;
    
    GLuint lineProgram;
    GLuint passthroughProgram;
    GLuint gaussianVProgram;
    GLuint gaussianHProgram;

    GLuint lineArrayBuffer;
    GLuint lineElementBuffer;
    GLuint lineVertexArray;

    GLuint passthroughBuffer;
    
    GLuint uniformLineViewport;
    GLuint uniformLineTime;
};


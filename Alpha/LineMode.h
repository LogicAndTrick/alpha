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
    
    shader::program lineProgram;
    shader::program quadsProgram;
    shader::program alphaLineProgram;
    shader::program passthroughProgram;
    shader::program gaussianVProgram;
    shader::program gaussianHProgram;

    GLuint lineArrayBuffer;
    GLuint lineElementBuffer;
    GLuint lineVertexArray;

    GLuint quadsBuffer;

    GLuint passthroughBuffer;
    
    GLuint uniformLineViewport;
    GLuint uniformLineTime;
    
    GLuint uniformQuadsViewport;
    GLuint uniformQuadsTime;

    GLuint sceneFrameBuffer;
    GLuint sceneFrameTexture;

    GLuint alphaSceneFrameBuffer;
    GLuint alphaSceneFrameTexture;

    GLuint gaussianHFrameBuffer;
    GLuint gaussianHFrameTexture;

    GLuint gaussianVFrameBuffer;
    GLuint gaussianVFrameTexture;
};


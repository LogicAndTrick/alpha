#pragma once

#include "glm/glm.hpp"
#include "gl/glew.h"
#include "shader.h"
#include "gamemode.h"
#include "level.h"

class ArrowMode : public GameMode
{
public:
    ArrowMode(void);
    ~ArrowMode(void);

    void Initialise();
    void Destroy();

    void Update();
    void Render();

    void OnMouseMove(int x, int y, int deltaX, int deltaY);
    void OnResize(int w, int h);
private:
    level level;
    long startTime;

    long pointCount;

    shader::program circlesProgram;
    GLuint uniformCirclesViewport;

    GLuint circlesArrayBuffer;
    GLuint circlesElementBuffer;
    GLuint circlesVertexArray;
};


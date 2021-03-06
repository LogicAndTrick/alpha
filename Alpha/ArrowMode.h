#pragma once

#include <SDL_image.h>
#include "glm/glm.hpp"
#include "gl/glew.h"
#include "shader.h"
#include "globals.h"
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
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnResize(int w, int h);
private:
    level level;
    long startTime;

    long pointCount;
    float width;
    float height;

    shader::program circlesProgram;
    GLuint uniformCirclesViewport;

    GLuint circlesArrayBuffer;
    GLuint circlesElementBuffer;
    GLuint circlesVertexArray;

    GLuint arrowTexture;
};


#pragma once

#include <SDL_image.h>
#include "glm/glm.hpp"
#include "gl/glew.h"
#include "shader.h"
#include "globals.h"
#include "gamemode.h"
#include "level.h"
#include "particle.h"

class ParticleMode : public GameMode
{
public:
    ParticleMode(void);
    ~ParticleMode(void);

    void Initialise();
    void Destroy();

    void Update();
    void Render();
    
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnResize(int w, int h);
private:
    shader::program program;
    GLuint uniformViewport;

    shader::program program2;
    GLuint uniformViewport2;

    ParticleEngine *engine;
};


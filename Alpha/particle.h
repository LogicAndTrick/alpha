#pragma once

#include <stdlib.h>
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "common.h"
#include "shader.h"

typedef struct {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 colour;
    GLfloat size;
    GLfloat age;
    GLfloat lifespan;
} particle;

class ParticleEffect {
public:
    
    ParticleEffect(shader::program program, long stepDuration, long currentTime, int numParticles);
    ~ParticleEffect();

    void Add(particle p);
    void Update(frame f);
    void Render();

    void (*ResetDeadParticle)(particle*);
    void (*UpdatePosition)(particle*, long duration);
    void (*UpdateColour)(particle*, long duration);
    void (*UpdateSize)(particle*, long duration);
private:
    void Step();

    int numParticles;
    int maxParticles;
    particle *particles;
    GLuint *indices;

    long stepDuration;
    shader::program program;

    long nextStepTime;
    
    GLuint arrayBuffer;
    GLuint vertexArray;
};
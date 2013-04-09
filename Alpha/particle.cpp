#include "particle.h"

void Particle_UpdatePositionDefault(particle* particle, long duration)
{
    float time = duration / 1000.0f;
    particle->position += particle->velocity * glm::vec2(time, time);
}

void Particle_DoNothingDefault(particle* particle, long duration)
{
}

void Particle_ResetNothingDefault(particle* particle)
{
}

ParticleEffect::ParticleEffect(shader::program program, long stepDuration, long currentTime, int numParticles)
{
    this->program = program;
    this->stepDuration = stepDuration;
    this->nextStepTime = currentTime;

    this->numParticles = 0;
    this->maxParticles = numParticles;
    this->particles = (particle*) malloc(sizeof(particle) * this->maxParticles);
    this->indices = (GLuint*) malloc(sizeof(GLuint) * this->maxParticles);

    for (int i = 0; i < this->maxParticles; i++) this->indices[i] = i;
    
    this->UpdatePosition = Particle_UpdatePositionDefault;
    this->UpdateColour = Particle_DoNothingDefault;
    this->UpdateSize = Particle_DoNothingDefault;
    this->ResetDeadParticle = Particle_ResetNothingDefault;

    glGenBuffers(1, &this->arrayBuffer);
    glGenVertexArrays(1, &this->vertexArray);

    glBindVertexArray(this->vertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(particle), 0); // position
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(particle), (GLvoid*) (sizeof(GLfloat) * 2)); // velocity
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(particle), (GLvoid*) (sizeof(GLfloat) * 4)); // colour
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (GLvoid*) (sizeof(GLfloat) * 8)); // size/age/lifespan
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleEffect::~ParticleEffect()
{
    free(this->particles);
    free(this->indices);
}

void ParticleEffect::Add(particle p)
{
    if (this->numParticles >= this->maxParticles) return;
    this->particles[this->numParticles++] = p;
}

void ParticleEffect::Update(frame f)
{
    while (f.tick > this->nextStepTime)
    {
        this->Step();
        this->nextStepTime += this->stepDuration;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle) * this->numParticles, this->particles, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleEffect::Render()
{
    shader::Bind(this->program);
    {
        glBindVertexArray(this->vertexArray);
        {
            glDrawArrays(GL_POINTS, 0, this->numParticles);
        }
        glBindVertexArray(0);
    }
    shader::Unbind();
}

void ParticleEffect::Step()
{
    for (int i = 0; i < this->numParticles; i++) {
        particle *p = this->particles + i;
        p->age += this->stepDuration;
        if (p->age > p->lifespan) {
            this->ResetDeadParticle(p);
        } else {
            this->UpdatePosition(p, this->stepDuration);
            this->UpdateColour(p, this->stepDuration);
            this->UpdateSize(p, this->stepDuration);
        }
    }
}
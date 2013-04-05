#include "particle.h"

void Particle_UpdatePositionDefault(particle* particle, long duration)
{
    float time = duration / 1000.0f;
    particle->position += particle->velocity * glm::vec2(time, time);
}

void Particle_DoNothingDefault(particle* particle, long duration)
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
    
    this->UpdatePosition = Particle_UpdatePositionDefault;
    this->UpdateColour = Particle_DoNothingDefault;
    this->UpdateSize = Particle_DoNothingDefault;

    srand(currentTime);

    glGenBuffers(1, &this->arrayBuffer);
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleEffect::~ParticleEffect()
{
    free(this->particles);
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
        glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
        {
            glDrawArrays(GL_POINTS, 0, this->numParticles);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    shader::Unbind();
}

void ParticleEffect::ResetDeadParticles(void (*Reset)(particle*))
{
    for (int i = 0; i < this->numParticles; i++) {
        particle *p = this->particles + i;
        if (p->age > p->lifespan) {
            Reset(p);
        }
    }
}

void ParticleEffect::Step()
{
    for (int i = 0; i < this->numParticles; i++) {
        particle *p = this->particles + i;
        this->UpdatePosition(p, this->stepDuration);
        this->UpdateColour(p, this->stepDuration);
        this->UpdateSize(p, this->stepDuration);
        p->age += this->stepDuration;
    }
}
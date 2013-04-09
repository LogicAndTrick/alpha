#include "ParticleMode.h"
#include "glm/gtc/matrix_transform.hpp"

ParticleMode::ParticleMode(void)
{

}

ParticleMode::~ParticleMode(void)
{

}

particle createParticle(
    float x, float y, 
    float vx, float vy, 
    float r, float g, float b,
    float size, long lifespan)
{
    particle p;
    p.lifespan = lifespan;
    p.age = 0;
    p.position.x = x;
    p.position.y = y;
    p.size = size;
    p.velocity.x = vx;
    p.velocity.y = vy;
    p.colour.r = r;
    p.colour.g = g;
    p.colour.b = b;
    p.colour.a = 1;
    return p;
}

float random() {
	return rand() / (float) RAND_MAX;
}

void resetParticle(particle *p)
{
    float x = 320;
    float y = 240;
    particle c = createParticle(
            x, // x
            y, // y
            (random()-0.5) * 320, // vx
            (random()-0.0) * -360, // vy
            0.5 + random() * 0.5, // r
            0.5 + random() * 0.5, // g
            0.5 + random() * 0.5, // b
            1 + random() * 1.5, // size
            1000 + random() * 4000 // lifespan
            );
    memcpy(p, &c, sizeof(particle));
}

particle createRandomParticle()
{
    particle p;
    resetParticle(&p);
    return p;
}

void ParticleMode_UpdatePosition(particle* particle, long duration)
{
    float time = duration / 1000.0f;
    particle->position += particle->velocity * glm::vec2(time, time);
    particle->velocity.y += 2; // gravity
    particle->velocity.x += (random()-0.5) * 5;
    particle->velocity.y += (random()-0.5) * 5;
}

void ParticleMode_UpdatePosition2(particle* particle, long duration)
{
    float time = duration / 1000.0f;
    particle->position += particle->velocity * glm::vec2(time, time);

    particle->position.x += (random() - 0.5) * 0.1;
    particle->position.y += (random() - 0.5) * 0.1;
    particle->size += (random() - 0.5) * 0.2;
    particle->velocity.x += (random() - 0.5) * 0.5;
    particle->velocity.y += (random() - 0.5) * 0.5;
}

void ParticleMode_Reset2(particle *p)
{
    p->age = 0;
    p->position.x += (random() - 0.5) * 5;
    p->position.y += (random() - 0.5) * 5;
    p->velocity.x = random() - 0.5;
    p->velocity.y = random() - 0.5;
    p->size = 1 + random() * 2;
    p->age = 0;
    p->lifespan = 1000 + random() * 2000;
}

void ParticleMode::Initialise()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create programs
    this->program = shader::LoadProgramFromFiles("shaders/particle.vert", "shaders/particle.geom", "shaders/particle_lines.frag");
    this->uniformViewport = glGetUniformLocation(this->program.id, "viewport");

    this->program2 = shader::LoadProgramFromFiles("shaders/particle.vert", "shaders/particle.geom", "shaders/particle_sparks.frag");
    this->uniformViewport2 = glGetUniformLocation(this->program2.id, "viewport");

    int numParticles = 1000;
    int numSparks = 500;

    // Create effects
    this->effect = new ParticleEffect(this->program, 10, this->currentFrame.tick - 4000, numParticles);
    this->effect->UpdatePosition = ParticleMode_UpdatePosition;
    this->effect->ResetDeadParticle = resetParticle;

    this->effect2 = new ParticleEffect(this->program2, 10, this->currentFrame.tick - 4000, numSparks);
    this->effect2->UpdatePosition = ParticleMode_UpdatePosition2;
    this->effect2->ResetDeadParticle = ParticleMode_Reset2;


    // Initialise effects
    for (int i = 0; i < numParticles; i++) {
        this->effect->Add(createRandomParticle());
    }

    float cx = 320, cy = 240, radius = 150;
    for (int i = 0; i < numSparks; i++) {
        particle p;
        float angle = (2 * 3.14159) * (i / (float) numSparks);
        p.position.x = cx + cos(angle) * radius + (random() - 0.5) * 100;
        p.position.y = cy + sin(angle) * radius + (random() - 0.5) * 100;
        p.velocity.x = random() - 0.5;
        p.velocity.y = random() - 0.5;
        p.colour.r = p.colour.g = p.colour.b = p.colour.a = 1;
        p.size = 1 + random() * 2;
        p.age = 0;
        p.lifespan = 1000 + random() * 2000;
        this->effect2->Add(p);
    }
    
    // Prepare effects
    this->effect->Update(this->currentFrame);
    this->effect2->Update(this->currentFrame);
}

void ParticleMode::Destroy()
{
    delete this->effect;
    shader::DestroyProgram(this->program);

    delete this->effect2;
    shader::DestroyProgram(this->program2);
}

void ParticleMode::Update()
{
    this->effect->Update(this->currentFrame);
    this->effect2->Update(this->currentFrame);
}

void ParticleMode::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->effect->Render();
    this->effect2->Render();
}

void ParticleMode::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void ParticleMode::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void ParticleMode::OnResize(int w, int h)
{
    // float ratio = h / (float) w;
    // this->width = 1.1;
    // this->height = 1.1 * ratio;
    glm::mat4 ortho = glm::ortho<float>(0, w, h, 0);

    shader::Bind(this->program);
    glUniformMatrix4fv(this->uniformViewport, 1, GL_FALSE, &ortho[0][0]);
    shader::Unbind();

    shader::Bind(this->program2);
    glUniformMatrix4fv(this->uniformViewport2, 1, GL_FALSE, &ortho[0][0]);
    shader::Unbind();
}
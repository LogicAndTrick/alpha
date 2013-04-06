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
    float y = -120;
    particle c = createParticle(
            x, // x
            y, // y
            (random()-0.5) * 320, // vx
            (random()-0.0) * -240, // vy
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

void ParticleMode::Initialise()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->program = shader::LoadProgramFromFiles("shaders/particle.vert", "shaders/particle.geom", "shaders/particle.frag");
    this->uniformViewport = glGetUniformLocation(this->program.id, "viewport");

    int numParticles = 2000;
    this->effect = new ParticleEffect(this->program, 10, this->currentFrame.tick - 4000, numParticles);
    this->effect->UpdatePosition = ParticleMode_UpdatePosition;
    this->effect->ResetDeadParticle = resetParticle;

    for (int i = 0; i < numParticles; i++) {
        this->effect->Add(createRandomParticle());
    }
    
    this->effect->Update(this->currentFrame);
}

void ParticleMode::Destroy()
{
    delete this->effect;
    shader::DestroyProgram(this->program);
}

void ParticleMode::Update()
{
    this->effect->Update(this->currentFrame);
}

void ParticleMode::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    this->effect->Render();
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
}
#include "ParticleMode.h"
#include "glm/gtc/matrix_transform.hpp"

ParticleMode::ParticleMode(void)
{

}

ParticleMode::~ParticleMode(void)
{

}

particle createParticle(float x, float y, float vx, float vy, float size, long lifespan)
{
    particle p;
    p.lifespan = lifespan;
    p.age = 0;
    p.position.x = x;
    p.position.y = y;
    p.size = size;
    p.velocity.x = vx;
    p.velocity.y = vy;
    return p;
}

float random() {
	return rand() / (float) RAND_MAX;
}

void resetParticle(particle *p)
{
    particle c = createParticle(
            random() * 640, // x
            random() * 480, // y
            (random()-0.5) * 30, // vx
            (random()-0.5) * 100, // vy
            1 + random() * 1.5, // size
            2000 + random() * 5000 // lifespan
            );
    memcpy(p, &c, sizeof(particle));
}

particle createRandomParticle()
{
    particle p;
    resetParticle(&p);
    return p;
}

void ParticleMode::Initialise()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->program = shader::LoadProgramFromFiles("shaders/particle.vert", "shaders/particle.geom", "shaders/particle.frag");
    this->uniformViewport = glGetUniformLocation(this->program.id, "viewport");

    int numParticles = 1000;
    this->effect = new ParticleEffect(this->program, 10, this->currentFrame.tick - 0, numParticles);

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
    this->effect->ResetDeadParticles(resetParticle);
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
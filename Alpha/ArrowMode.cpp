#include "ArrowMode.h"
#include "glm/gtc/matrix_transform.hpp"

ArrowMode::ArrowMode(void)
{
    this->level.totalLength = 10000;
    this->level.visibleLength = 2000;
    this->level.runoffLength = 200;
    this->level.leadInLength = 2000;
    this->level.numArrows = 8;
    this->level.arrows = (arrow*) malloc(sizeof(arrow) * this->level.numArrows);

    for (int i = 0; i < this->level.numArrows; i++) {
        (this->level.arrows+i)->type = 0;
        (this->level.arrows+i)->lane = i % 4;
        (this->level.arrows+i)->direction = i % 8;
        (this->level.arrows+i)->position = i * 1000;
        (this->level.arrows+i)->visible = 1;
    }
}

ArrowMode::~ArrowMode(void)
{
    free(this->level.arrows);
}

void ArrowMode::Initialise()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->startTime = this->currentFrame.tick + this->level.leadInLength;
    
    this->circlesProgram = shader::LoadProgramFromFiles("shaders/circles.vert", "shaders/circles.frag");
    this->uniformCirclesViewport = glGetUniformLocation(this->circlesProgram.id, "viewport");

    glGenBuffers(1, &this->circlesArrayBuffer);
    glGenBuffers(1, &this->circlesElementBuffer);
    glGenVertexArrays(1, &this->circlesVertexArray);

    glBindVertexArray(this->circlesVertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->circlesArrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*) (sizeof(GLfloat) * 2));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*) (sizeof(GLfloat) * 4));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->circlesElementBuffer);
    }
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    char path[260] = "";
    globals::ResolvePath("textures/arrow.png", path);

    SDL_Surface *surface = IMG_Load(path);
    glGenTextures(1, &this->arrowTexture);
    glBindTexture(GL_TEXTURE_2D, this->arrowTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
}

void ArrowMode::Destroy()
{
    glDeleteTextures(1, &this->arrowTexture);

    glDeleteVertexArrays(1, &this->circlesVertexArray);
    glDeleteBuffers(1, &this->circlesArrayBuffer);
    glDeleteBuffers(1, &this->circlesElementBuffer);

    shader::DestroyProgram(this->circlesProgram);
}

void ArrowMode::Update()
{
    int currentTime = this->currentFrame.tick - this->startTime;
    int addLength = (this->level.runoffLength + this->level.visibleLength) * 0.2;
    int minimum = (currentTime - this->level.runoffLength) - addLength;
    int maximum = (currentTime + this->level.visibleLength) + addLength;
    long start = -1, end = -1, count = 0, i;
    arrow *p;
    for (i = 0; i < this->level.numArrows; i++) {
        p = this->level.arrows + i;
        if (p->position > maximum) break;
        if (p->position < minimum || !p->visible) continue;
        if (start < 0) start = i;
        end = i;
        count++;
    }

    this->pointCount = count;

    GLfloat *adata = (GLfloat*) malloc(4 * count * 8 * sizeof(GLfloat));
    GLuint *idata = (GLuint*) malloc(4 * count * sizeof(GLuint));
    GLfloat *aptr = adata;
    GLuint *iptr = idata;

    if (start < 0 || end < 0 || end < start) return;

    GLuint c = 0;
    for (i = start; i <= end; i++) {
        p = this->level.arrows + i;

        GLfloat x, y;
        
        // X
        if (p->position < currentTime) { // > 0
            x = (currentTime - p->position) / (float) this->level.runoffLength * 0.1;
        } else { // <= 0
            x = (currentTime - p->position) / (float) this->level.visibleLength;
        }
        
        // Y
        y = (p->lane + 1) * (this->height / 5);

        for (int j = 0; j < 4; j++) {
            
            const float xd[] = { -0.05, 0.05, 0.05, -0.05 };
            const float yd[] = { -0.05, -0.05, 0.05, 0.05 };
            
            const float tu[] = {
                0, 1, 1, 0,
                0, 0, 1, 1,
                1, 0, 0, 1,
                1, 1, 0, 0,
                -0.207107, 0.5, 1.207107, 0.5,
                0.5, -0.207107, 0.5, 1.207107,
                1.207107, 0.5, -0.207107, 0.5,
                0.5, 1.207107, 0.5, -0.207107
            };
            const float tv[] = {
                0, 0, 1, 1,
                1, 0, 0, 1,
                1, 1, 0, 0,
                0, 1, 1, 0,
                0.5, -0.207107, 0.5, 1.207107,
                1.207107, 0.5, -0.207107, 0.5,
                0.5, 1.207107, 0.5, -0.207107,
                -0.207107, 0.5, 1.207107, 0.5
            };

            *iptr++ = c++;

            *aptr++ = x + xd[j];
            *aptr++ = y + yd[j];

            *aptr++ = tu[p->direction * 4 + j]; // U
            *aptr++ = tv[p->direction * 4 + j]; // V

            *aptr++ = 1.0f; // R
            *aptr++ = 1.0f; // G
            *aptr++ = 1.0f; // B
            *aptr++ = 1.0f; // A
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->circlesArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * count * 8, adata, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->circlesElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 4 * count, idata, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(adata);
    free(idata);
}

void ArrowMode::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, this->arrowTexture);
    shader::Bind(this->circlesProgram);
    {
        glBindVertexArray(this->circlesVertexArray);
        {
            long inum = this->pointCount * 4;
            glDrawElements(GL_QUADS, inum, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    }
    shader::Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ArrowMode::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    int dir = -1;
    switch (sym) {
    case SDLK_KP8: dir = 0; break;
    case SDLK_KP6: dir = 1; break;
    case SDLK_KP2: dir = 2; break;
    case SDLK_KP4: dir = 3; break;
    case SDLK_KP9: dir = 4; break;
    case SDLK_KP3: dir = 5; break;
    case SDLK_KP1: dir = 6; break;
    case SDLK_KP7: dir = 7; break;
    }
    if (dir < 0) return;
    
    int currentTime = this->currentFrame.tick - this->startTime;
    int minimum = currentTime - 200;
    int maximum = currentTime + 200;
    arrow *p;
    for (int i = 0; i < this->level.numArrows; i++) {
        p = this->level.arrows + i;
        if (p->position > minimum && p->position < maximum && p->visible) break;
        p = 0;
    }
    if (!p) return;

    if (p->direction == dir) {
        printf("HIT\r\n");
    } else {
        printf("MISS\r\n");
    }
}

void ArrowMode::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void ArrowMode::OnResize(int w, int h)
{
    float ratio = h / (float) w;
    this->width = 1.1;
    this->height = 1.1 * ratio;
    glm::mat4 ortho = glm::ortho<float>(-1, 0.1, this->height, 0);

    shader::Bind(this->circlesProgram);
    glUniformMatrix4fv(this->uniformCirclesViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);
}
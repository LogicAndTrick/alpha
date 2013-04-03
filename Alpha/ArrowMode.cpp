#include "ArrowMode.h"
#include "glm/gtc/matrix_transform.hpp"

ArrowMode::ArrowMode(void)
{
    this->level.totalLength = 10000;
    this->level.visibleLength = 2000;
    this->level.runoffLength = 2000;
    this->level.leadInLength = 1000;
    this->level.numArrows = 50;
    this->level.arrows = (arrow*) malloc(sizeof(arrow) * this->level.numArrows);

    for (int i = 0; i < this->level.numArrows; i++) {
        (this->level.arrows+i)->type = 0;
        (this->level.arrows+i)->lane = i % 4;
        (this->level.arrows+i)->position = i * 200;
    }
}

ArrowMode::~ArrowMode(void)
{
    free(this->level.arrows);
}

void ArrowMode::Initialise()
{
    glClearColor(0, 0, 0, 0);

    this->startTime = this->currentFrame.tick + this->level.leadInLength;
    
    this->circlesProgram = shader::LoadProgramFromFiles("shaders/circles.vert", "shaders/circles.frag");
    this->uniformCirclesViewport = glGetUniformLocation(this->circlesProgram.id, "viewport");

    glm::mat4 ortho = glm::ortho<float>(-1, 0.1, 1, 0);

    shader::Bind(this->circlesProgram);
    glUniformMatrix4fv(this->uniformCirclesViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);

    glGenBuffers(1, &this->circlesArrayBuffer);
    glGenBuffers(1, &this->circlesElementBuffer);
    glGenVertexArrays(1, &this->circlesVertexArray);

    glBindVertexArray(this->circlesVertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->circlesArrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*) (sizeof(GLfloat) * 2));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->circlesElementBuffer);
    }
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ArrowMode::Destroy()
{
    glDeleteVertexArrays(1, &this->circlesVertexArray);
    glDeleteBuffers(1, &this->circlesArrayBuffer);
    glDeleteBuffers(1, &this->circlesElementBuffer);

    shader::DestroyProgram(this->circlesProgram);
}

void ArrowMode::Update()
{
    int currentTime = this->currentFrame.tick - this->startTime;
    int minimum = currentTime - this->level.runoffLength;
    int maximum = currentTime + this->level.visibleLength;
    long start = -1, end = -1, count = 0, i;
    arrow *p;
    for (i = 0; i < this->level.numArrows; i++) {
        p = this->level.arrows + i;
        if (p->position > maximum) break;
        if (p->position < minimum) continue;
        if (start < 0) start = i;
        end = i;
        count++;
    }

    this->pointCount = count;

    GLfloat *adata = (GLfloat*) malloc(count * 6 * sizeof(GLfloat));
    GLuint *idata = (GLuint*) malloc(count * sizeof(GLuint));
    GLfloat *aptr = adata;
    GLuint *iptr = idata;

    if (start < 0 || end < 0 || end < start) return;

    GLuint c;
    for (i = start, c = 0; i <= end; i++, c++) {
        p = this->level.arrows + i;

        *iptr++ = c;
        
        // X
        if (p->position < currentTime) { // > 0
            *aptr++ = (currentTime - p->position) / (float) this->level.runoffLength * 0.1;
        } else { // <= 0
            *aptr++ = (currentTime - p->position) / (float) this->level.visibleLength;
        }
        
        // Y
        *aptr++ = 0.5f;

        *aptr++ = 1.0f; // R
        *aptr++ = 1.0f; // G
        *aptr++ = 1.0f; // B
        *aptr++ = 1.0f; // A
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->circlesArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count * 6, adata, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->circlesElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, idata, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(adata);
    free(idata);
}

void ArrowMode::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader::Bind(this->circlesProgram);
    {
        glBindVertexArray(this->circlesVertexArray);
        {
            long inum = this->pointCount;
            glDrawElements(GL_POINTS, inum, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    }
    shader::Unbind();
}

void ArrowMode::OnMouseMove(int x, int y, int deltaX, int deltaY)
{

}

void ArrowMode::OnResize(int w, int h)
{

}

#include <stdlib.h>
#include "glm/gtc/matrix_transform.hpp"

#include "globals.h"
#include "LineMode.h"


LineMode::LineMode(void)
{
    this->points = new std::list<glm::vec3>();
}


LineMode::~LineMode(void)
{
    delete this->points;
}

void LineMode::Initialise()
{
    glLineWidth(4);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    char defaultVert[256] = "";
    char defaultFrag[256] = "";
    globals::ResolvePath("shaders/default.vert", defaultVert);
    globals::ResolvePath("shaders/default.frag", defaultFrag);
    
    this->defaultVertShader = shader::LoadFromFile(defaultVert, GL_VERTEX_SHADER);
    this->defaultFragShader = shader::LoadFromFile(defaultFrag, GL_FRAGMENT_SHADER);
    this->defaultProgram = shader::CreateProgram(this->defaultVertShader, this->defaultFragShader);
    
    this->uniformViewport = glGetUniformLocation(this->defaultProgram, "viewport");
    this->uniformTime = glGetUniformLocation(this->defaultProgram, "time");
    glm::mat4 ortho = glm::ortho<float>(0, 640, 480, 0);

    glUseProgram(this->defaultProgram);
    glUniformMatrix4fv(this->uniformViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);
    
    glGenBuffers(1, &this->arrayBuffer);
    glGenBuffers(1, &this->elementBuffer);
    glGenVertexArrays(1, &this->vertexArray);

    glBindVertexArray(this->vertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*) (sizeof(float) * 2));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LineMode::Destroy()
{
    glDeleteVertexArrays(1, &this->vertexArray);
    glDeleteBuffers(1, &this->arrayBuffer);
    glDeleteBuffers(1, &this->elementBuffer);

    glDeleteProgram(this->defaultProgram);
    glDeleteShader(this->defaultVertShader);
    glDeleteShader(this->defaultFragShader);
}

void LineMode::Update()
{
    int anum = this->points->size() * 3;
    int inum = (this->points->size() - 1) * 2;
    GLfloat *adata = (GLfloat*) malloc(anum * sizeof(GLfloat));
    GLuint *idata = (GLuint*) malloc(inum * sizeof(GLuint));
    GLfloat *aptr = adata;
    GLuint *iptr = idata;
    GLuint count = 0;
    GLfloat size = this->points->size();
    for (std::list<glm::vec3>::iterator it = this->points->begin(); it != this->points->end(); ++it) {
        if (count > 0) {
            *iptr++ = count - 1;
            *iptr++ = count;
        }
        *aptr++ = it->x;
        *aptr++ = it->y;
        *aptr++ = it->z;
        count++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * anum, adata, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inum, idata, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(adata);
    free(idata);
}

void LineMode::Render()
{
    glUseProgram(this->defaultProgram);
    {
        float time = this->currentFrame.tick / 1000.0;
        glUniform1f(this->uniformTime, time);
        glBindVertexArray(this->vertexArray);
        {
            int inum = (this->points->size() - 1) * 2;
            glDrawElements(GL_LINES, inum, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    }
    glUseProgram(0);

    return;

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glm::vec2 b;
    bool first = true;
    for (std::list<glm::vec3>::iterator it = this->points->begin(); it != this->points->end(); ++it) {
        if (!first) {
            glVertex2f(b.x, b.y);
            glVertex2f(it->x, it->y);
        }
        first = false;
        b.x = it->x;
        b.y = it->y;
    }
    glEnd();
}

void LineMode::OnMouseMove(int x, int y, int deltaX, int deltaY)
{
    float time = this->currentFrame.tick / 1000.0;
    this->points->push_front(glm::vec3(x, y, time));
    if (this->points->size() > LineMode::MAX_POINTS) this->points->pop_back();
}

void LineMode::OnResize(int w, int h)
{

}
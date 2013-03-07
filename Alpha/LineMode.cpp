
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
    glClearColor(0, 0, 0, 0);
    glLineWidth(4);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    char path[260] = "";

    globals::ResolvePath("shaders/line.vert", path);
    this->lineVertShader = shader::LoadFromFile(path, GL_VERTEX_SHADER);

    globals::ResolvePath("shaders/line.frag", path);
    this->lineFragShader = shader::LoadFromFile(path, GL_FRAGMENT_SHADER);

    globals::ResolvePath("shaders/passthrough.vert", path);
    this->passthroughVertShader = shader::LoadFromFile(path, GL_VERTEX_SHADER);

    globals::ResolvePath("shaders/passthrough.frag", path);
    this->passthroughFragShader = shader::LoadFromFile(path, GL_FRAGMENT_SHADER);

    globals::ResolvePath("shaders/gaussian_hor.frag", path);
    this->gaussianHFragShader = shader::LoadFromFile(path, GL_FRAGMENT_SHADER);

    globals::ResolvePath("shaders/gaussian_vert.frag", path);
    this->gaussianVFragShader = shader::LoadFromFile(path, GL_FRAGMENT_SHADER);
    
    this->lineProgram = shader::CreateProgram(this->lineVertShader, this->lineFragShader);
    this->passthroughProgram = shader::CreateProgram(this->passthroughVertShader, this->passthroughFragShader);
    this->gaussianHProgram = shader::CreateProgram(this->passthroughVertShader, this->gaussianHFragShader);
    this->gaussianVProgram = shader::CreateProgram(this->passthroughVertShader, this->gaussianVFragShader);
    
    this->uniformLineViewport = glGetUniformLocation(this->lineProgram, "viewport");
    this->uniformLineTime = glGetUniformLocation(this->lineProgram, "time");
    glm::mat4 ortho = glm::ortho<float>(0, 640, 480, 0);

    glUseProgram(this->lineProgram);
    glUniformMatrix4fv(this->uniformLineViewport, 1, GL_FALSE, &ortho[0][0]);
    glUseProgram(0);
    
    glGenBuffers(1, &this->lineArrayBuffer);
    glGenBuffers(1, &this->lineElementBuffer);
    glGenVertexArrays(1, &this->lineVertexArray);

    glBindVertexArray(this->lineVertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->lineArrayBuffer);
        {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*) (sizeof(float) * 2));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->lineElementBuffer);
    }
    glBindVertexArray(0);

    glGenBuffers(1, &this->passthroughBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->passthroughBuffer);
    {
        GLfloat data[] = { 1.f, 1.f, -1.f, 1.f, -1.f,-1.f,
					      -1.f,-1.f,  1.f,-1.f,  1.f, 1.f };
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, data, GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LineMode::Destroy()
{
    glDeleteVertexArrays(1, &this->lineVertexArray);
    glDeleteBuffers(1, &this->lineArrayBuffer);
    glDeleteBuffers(1, &this->lineElementBuffer);
    
    glDeleteProgram(this->lineProgram);
    glDeleteProgram(this->passthroughProgram);
    glDeleteProgram(this->gaussianHProgram);
    glDeleteProgram(this->gaussianVProgram);

    glDeleteShader(this->lineVertShader);
    glDeleteShader(this->lineFragShader);
    glDeleteShader(this->passthroughFragShader);
    glDeleteShader(this->passthroughVertShader);
    glDeleteShader(this->gaussianHFragShader);
    glDeleteShader(this->gaussianVFragShader);
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

    glBindBuffer(GL_ARRAY_BUFFER, this->lineArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * anum, adata, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->lineElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inum, idata, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    free(adata);
    free(idata);
}

void LineMode::Render()
{
    glUseProgram(this->lineProgram);
    {
        float time = this->currentFrame.tick / 1000.0;
        glUniform1f(this->uniformLineTime, time);
        glBindVertexArray(this->lineVertexArray);
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

#include "globals.h"
#include "LineMode.h"


LineMode::LineMode(void)
{
    this->points = new std::list<glm::vec2>();
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
    
    glGenBuffers(1, &this->arrayBuffer);
    glGenBuffers(1, &this->elementBuffer);
    glGenVertexArrays(1, &this->vertexArray);

    glBindVertexArray(this->vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, this->arrayBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBuffer);
    glBindVertexArray(0);
}

void LineMode::Destroy()
{
    glDeleteProgram(this->defaultProgram);
    glDeleteShader(this->defaultVertShader);
    glDeleteShader(this->defaultFragShader);
}

void LineMode::Update()
{
}

void LineMode::Render()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glm::vec2 b;
    bool first = true;
    for (std::list<glm::vec2>::iterator it = this->points->begin(); it != this->points->end(); ++it) {
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
    this->points->push_front(glm::vec2(x, y));
    if (this->points->size() > LineMode::MAX_POINTS) this->points->pop_back();
}

void LineMode::OnResize(int w, int h)
{

}
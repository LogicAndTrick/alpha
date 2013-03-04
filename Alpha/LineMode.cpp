#include <windows.h>
#include <gl/gl.h>
#include <cstdlib>

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
}

void LineMode::Destroy()
{
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
            glVertex2i(b.x, b.y);
            glVertex2i(it->x, it->y);
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
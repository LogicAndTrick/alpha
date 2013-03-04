#include <windows.h>
#include <gl/gl.h>
#include <cstdlib>

#include "LineMode.h"


LineMode::LineMode(void)
{
}


LineMode::~LineMode(void)
{
}

void LineMode::Initialise()
{
}

void LineMode::Destroy()
{
}

void LineMode::Update()
{
}

void LineMode::Render()
{
    glBegin(GL_QUADS);
        glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
        glColor3f(1, 1, 0); glVertex3f(100, 0, 0);
        glColor3f(1, 0, 1); glVertex3f(100, 100, 0);
        glColor3f(1, 1, 1); glVertex3f(0, 100, 0);
    glEnd();
}

void LineMode::OnMouseMove(int x, int y, int deltaX, int deltaY)
{
    printf("Mouse: %d, %d", x, y);
}

void LineMode::OnResize(int w, int h)
{

}
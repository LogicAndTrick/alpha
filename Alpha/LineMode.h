#pragma once

#include <list>
#include "glm/glm.hpp"
#include "gamemode.h"

class LineMode : public GameMode
{
public:
    LineMode(void);
    ~LineMode(void);

    void Initialise();
    void Destroy();

    void Update();
    void Render();

    void OnMouseMove(int x, int y, int deltaX, int deltaY);
    void OnResize(int w, int h);
private:
    std::list<glm::vec2> *points;
    static const int MAX_POINTS = 50;
};


#pragma once

typedef struct {
    int type;
    int direction;
    int lane;
    long position;
    int visible;
} arrow;

typedef struct {
    long totalLength;
    long visibleLength;
    long runoffLength;
    long leadInLength;
    long numArrows;
    arrow *arrows;
} level;
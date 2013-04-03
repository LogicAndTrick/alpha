#pragma once

typedef struct {
    int type;
    int lane;
    long position;
} arrow;

typedef struct {
    long totalLength;
    long visibleLength;
    long runoffLength;
    long leadInLength;
    long numArrows;
    arrow *arrows;
} level;
#pragma once

#include <string.h>

namespace globals {

    extern char BaseDirectory[260];
    extern char ExecutableName[260];

    void SetPath(const char *path);

    void ResolvePath(const char *relative, char *str);
}
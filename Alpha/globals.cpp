#include "globals.h"

namespace globals {

    char BaseDirectory[260];
    char ExecutableName[260];

    void SetPath(const char *path) {
        const char *pos = strrchr(path, '/');
        if (!pos) pos = strrchr(path, '\\');
        pos++;
        strncpy(BaseDirectory, path, pos - path);
        strncpy(ExecutableName, pos, strlen(pos));
    }

    void ResolvePath(const char *relative, char *str) {
        if (relative[0] == '/' || relative[0] == '\\') relative++;
        strcat(str, BaseDirectory);
        strcat(str, relative);
    }
}
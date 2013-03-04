
#include <cstdlib>
#include "gl/glew.h"
#include <SDL.h>

#include "Loop.h"
#include "LineMode.h"

int main ( int argc, char** argv )
{
    GameMode *mode = new LineMode();
    Loop *loop = new Loop(mode);
    int code = loop->Run();
    if (code != 0) {
        printf("Exited with error code: %d\n", code);
    } else {
        printf("Exited cleanly.\n");
    }
    delete loop;
    delete mode;
    return 0;
}


#include <cstdlib>
#include "gl/glew.h"
#include <SDL.h>

#include "globals.h"
#include "Loop.h"
#include "LineMode.h"
#include "ArrowMode.h"
#include "ParticleMode.h"

int main ( int argc, char** argv )
{
    globals::SetPath(argv[0]);

    GameMode *mode;
    //mode = new LineMode();
    //mode = new ArrowMode();
    mode = new ParticleMode();
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

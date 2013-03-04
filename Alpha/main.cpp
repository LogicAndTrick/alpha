#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
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

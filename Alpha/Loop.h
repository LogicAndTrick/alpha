#ifndef LOOP_H
#define LOOP_H

#include <SDL.h>

#include "GameMode.h"

typedef struct {
    long frame;
    long tick;
    long duration;
} frame;

class Loop
{
    public:
        Loop(GameMode *startMode);
        virtual ~Loop();
        int Run();
    protected:
        bool Initialise();
        void ProcessEvent(SDL_Event *Event);
        void Update();
        void Render();
        void Destroy();
        void SetGameMode(GameMode *gameMode);
    private:
        void SwitchModes();
        bool running;
        SDL_Surface *display;
        GameMode *mode;
        GameMode *switchMode;
};

#endif // LOOP_H

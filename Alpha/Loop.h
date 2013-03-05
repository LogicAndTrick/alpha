#pragma once

#include <SDL.h>
#include "common.h"
#include "GameMode.h"

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
        frame currentFrame;
};

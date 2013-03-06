#pragma once

#include <SDL.h>
#include "common.h"

enum Buttons {
    Left = SDL_BUTTON_LEFT,
    Right = SDL_BUTTON_RIGHT,
    Middle = SDL_BUTTON_MIDDLE,
    X1 = SDL_BUTTON_X1,
    X2 = SDL_BUTTON_X2
};

class GameMode
{
public:
    virtual void Initialise() = 0;
    virtual void Destroy() = 0;

    virtual void Update() = 0;
    virtual void Render() = 0;
    
    virtual void OnMouseFocus();
    virtual void OnMouseBlur();
    virtual void OnInputFocus();
    virtual void OnInputBlur();
    virtual void OnMinimize();
    virtual void OnRestore();

    virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    virtual void OnMouseMove(int x, int y, int deltaX, int deltaY);
    virtual void OnMouseWheel(int delta, int x, int y);
    virtual void OnMouseButtonDown(Buttons button, int x, int y);
    virtual void OnMouseButtonUp(Buttons button, int x, int y);
 
    virtual void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
    virtual void OnJoyButtonDown(Uint8 which, Uint8 button);
    virtual void OnJoyButtonUp(Uint8 which, Uint8 button);
    virtual void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
    virtual void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);
 
    virtual void OnResize(int w,int h);
    virtual void OnExpose();

    virtual void OnExit();
 
    virtual void OnUser(Uint8 type, int code, void* data1, void* data2);

    void SetFrame(frame f);
protected:
    frame currentFrame;
};

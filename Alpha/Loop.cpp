#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Loop.h"

Loop::Loop(GameMode *startMode)
{
    this->mode = startMode;
    this->switchMode = NULL;
}

Loop::~Loop()
{
    //dtor
}

int Loop::Run()
{
    if (!this->Initialise()) {
        return -1;
    }
    this->running = true;
    SDL_Event event;
    while (this->running) {
        while (SDL_PollEvent(&event)) {
            this->ProcessEvent(&event);
        }
        this->Update();
        this->Render();
        this->SwitchModes();
    }
    this->Destroy();
    return 0;
}

void Loop::SwitchModes()
{
    if (this->switchMode != NULL) {
        this->mode->Destroy();
        this->mode = this->switchMode;
        this->switchMode = NULL;
        this->mode->Initialise();
    }
}

void Loop::SetGameMode(GameMode *gameMode)
{
    this->switchMode = gameMode;
}

bool Loop::Initialise()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
    this->display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_OPENGL);
    if (this->display == NULL) return false;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glClearColor(0, 0, 0, 0);

    glViewport(0, 0, 640, 480);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, 640, 480, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);

    glLoadIdentity();

    return true;
}

// http://www.sdltutorials.com/sdl-events

void Loop::ProcessEvent(SDL_Event* event)
{
    GameMode *m = this->mode;
    switch (event->type) {
        case SDL_ACTIVEEVENT:
            switch (event->active.state) {
                case SDL_APPMOUSEFOCUS:
                    if (event->active.gain) m->OnMouseFocus();
                    else m->OnMouseBlur();
                    break;
                case SDL_APPINPUTFOCUS:
                    if (event->active.gain) m->OnInputFocus();
                    else m->OnInputBlur();
                    break;
                case SDL_APPACTIVE:
                    if (event->active.gain) m->OnRestore();
                    else m->OnMinimize();
                    break;
            }
            break;
        case SDL_KEYDOWN:
            m->OnKeyDown(event->key.keysym.sym, event->key.keysym.mod, event->key.keysym.unicode);
            break;
        case SDL_KEYUP:
            m->OnKeyUp(event->key.keysym.sym, event->key.keysym.mod, event->key.keysym.unicode);
            break;
        case SDL_MOUSEMOTION:
            m->OnMouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_WHEELDOWN) m->OnMouseWheel(-1, event->button.x, event->button.y);
            else if (event->button.button == SDL_BUTTON_WHEELUP) m->OnMouseWheel(1, event->button.x, event->button.y);
            else m->OnMouseButtonDown((Buttons)event->button.button, event->button.x, event->button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            m->OnMouseButtonDown((Buttons)event->button.button, event->button.x, event->button.y);
            break;
        case SDL_JOYAXISMOTION:
            m->OnJoyAxis(event->jaxis.which, event->jaxis.axis, event->jaxis.value);
            break;
        case SDL_JOYBALLMOTION:
            m->OnJoyBall(event->jball.which, event->jball.ball, event->jball.xrel, event->jball.yrel);
            break;
        case SDL_JOYHATMOTION:
            m->OnJoyHat(event->jhat.which, event->jhat.hat, event->jhat.value);
            break;
        case SDL_JOYBUTTONDOWN:
            m->OnJoyButtonDown(event->jbutton.which, event->jbutton.button);
            break;
        case SDL_JOYBUTTONUP:
            m->OnJoyButtonUp(event->jbutton.which, event->jbutton.button);
            break;
        case SDL_VIDEORESIZE:
            m->OnResize(event->resize.w, event->resize.h);
            break;
        case SDL_VIDEOEXPOSE:
            m->OnExpose();
            break;
        case SDL_QUIT:
            this->running = false;
            break;
        default:
            m->OnUser(event->user.type, event->user.code, event->user.data1, event->user.data2);
            break;
    }
}

void Loop::Update()
{
    this->mode->Update();
}

void Loop::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    this->mode->Render();

    SDL_GL_SwapBuffers();
}

void Loop::Destroy()
{
    SDL_FreeSurface(this->display);
}


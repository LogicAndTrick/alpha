#include "GameMode.h"

void GameMode::OnInputFocus() {
    // Virtual
}

void GameMode::OnInputBlur() {
    // Virtual
}


void GameMode::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    // Virtual
}

void GameMode::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
    // Virtual
}


void GameMode::OnMouseFocus() {
    // Virtual
}

void GameMode::OnMouseBlur() {
    // Virtual
}

void GameMode::OnMouseMove(int x, int y, int deltaX, int deltaY) {
    // Virtual
}

void GameMode::OnMouseWheel(int delta, int x, int y) {
    // Virtual
}

void GameMode::OnMouseButtonDown(Buttons button, int x, int y) {
    // Virtual
}

void GameMode::OnMouseButtonUp(Buttons button, int x, int y) {
    // Virtual
}


void GameMode::OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value) {
    // Virtual
}

void GameMode::OnJoyButtonDown(Uint8 which, Uint8 button) {
    // Virtual
}

void GameMode::OnJoyButtonUp(Uint8 which, Uint8 button) {
    // Virtual
}

void GameMode::OnJoyHat(Uint8 which, Uint8 hat, Uint8 value) {
    // Virtual
}

void GameMode::OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel) {
    // Virtual
}


void GameMode::OnMinimize() {
    // Virtual
}

void GameMode::OnRestore() {
    // Virtual
}


void GameMode::OnResize(int w,int h) {
    // Virtual
}

void GameMode::OnExpose() {
    // Virtual
}


void GameMode::OnExit() {
    // Virtual
}


void GameMode::OnUser(Uint8 type, int code, void* data1, void* data2) {
    // Virtual
}

void GameMode::SetFrame(frame f) {
    this->currentFrame = f;
}
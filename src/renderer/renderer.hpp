#pragma once
#include <SDL3/SDL.h>

class Renderer {
private:
    SDL_Window* m_window;
    SDL_GPUDevice* m_GPUDevice;

public:
    Renderer(SDL_Window* _window);
};
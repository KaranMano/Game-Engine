#pragma once

#include <SDL3/SDL.h>

#include <engine/ui.hpp>

class Engine {
private:
  UI m_ui;

public:
  Engine();
  ~Engine();

  void iterate();
  void handleEvent(SDL_Event* event);
};

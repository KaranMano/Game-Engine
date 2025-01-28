#pragma once

#include <SDL3/SDL.h>
#include <RmlUi/Core.h>

class UI {
private:
  Rml::ElementDocument *m_document;
  Rml::Context *m_context;

public:
  UI();
  ~UI();
  
  void render();
  void handleEvent(SDL_Event *event);
};
#include <engine/engine.hpp>

Engine::Engine() : m_ui() {};
Engine::~Engine() {};

void Engine::iterate() { m_ui.render(); }
void Engine::handleEvent(SDL_Event* event) { m_ui.handleEvent(event); }

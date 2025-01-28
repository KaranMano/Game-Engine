#include <engine/ui.hpp>

#ifdef GAME_ENGINE_DEBUG_MODE
#include <RmlUi/Debugger.h>
#endif

#include <renderer/ui-backend.hpp>
#include <renderer/ui-platform.hpp>

UI::UI() {
  if (not Backend::Initialize("Game Engine", 500, 500, true)) {
    throw std::exception("ui backend intialization failed");
  }
  Rml::SetSystemInterface(Backend::GetSystemInterface());
  Rml::SetRenderInterface(Backend::GetRenderInterface());
  if (not Rml::Initialise()) {
    throw std::exception("rml intialization failed");
  }

  m_context = Rml::CreateContext("main", Rml::Vector2i(500, 500),
                                 Backend::GetRenderInterface());
  if (not m_context) {
    Rml::Shutdown();
    Backend::Shutdown();
    throw std::exception("context intialization failed");
  }
#ifdef GAME_ENGINE_DEBUG_MODE
  if (not Rml::Debugger::Initialise(m_context)) {
    throw std::exception("ui debugger intialization failed");
  }
  Rml::Debugger::SetVisible(true);
#endif
  m_document = m_context->LoadDocument("../documents/root.rml");
  if (m_document) {
    m_document->Show();
  } else {
    Rml::Shutdown();
    Backend::Shutdown();
    throw std::exception("document intialization failed");
  }
}

UI::~UI() {
  // All raw pointers are non-owning, never call delete on objects returned by
  // the rmlui library with few exceptions
  m_document->Close();
  Rml::Debugger::Shutdown();
  Rml::RemoveContext(m_context->GetName());
  Rml::Shutdown();
  Backend::Shutdown();
}

void UI::render() {
  Backend::BeginFrame();
  m_context->Render();
  Backend::PresentFrame();
}

void UI::handleEvent(SDL_Event *event) {
  RmlSDL::InputEventHandler(m_context, Backend::GetWindow(), *event);
  m_context->Update();
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <engine/engine.hpp>

struct AppContext {
  Engine *engine;
  SDL_AppResult app_quit = SDL_APP_CONTINUE;
};

SDL_AppResult SDL_Fail() {
  SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
  return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    return SDL_Fail();
  }

  if (not TTF_Init()) {
    return SDL_Fail();
  }

  try {
    Engine *engine = new Engine();
    *appstate = new AppContext{
        .engine = engine,
    };
  }
  catch (std::exception &e) {
    SDL_Log(e.what());
    return SDL_Fail();
  } 
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  auto *app = static_cast<AppContext *>(appstate);
  if (event->type == SDL_EVENT_QUIT) {
    app->app_quit = SDL_APP_SUCCESS;
  }
  try {
    app->engine->handleEvent(event);
  } catch (std::exception &e) {
    SDL_Log(e.what());
    return SDL_Fail();
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto *app = static_cast<AppContext *>(appstate);
  try {
    app->engine->iterate();
  } catch (std::exception &e) {
    SDL_Log(e.what());
    return SDL_Fail();
  }
  return app->app_quit;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  auto *app = static_cast<AppContext *>(appstate);
  if (app) {
    delete app->engine;
    delete app;
  }

  TTF_Quit();
  SDL_Quit();
}

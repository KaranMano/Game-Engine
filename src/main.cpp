#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_assert.h>

#include <cmath>
#include <string_view>
#include <filesystem>

#include <cmake.hpp>
#include <engine/engine.hpp>
#include <renderer/renderer.hpp>
#include <physics/physics-engine.hpp>
#include <audio/audio-engine.hpp>

#include <glm/common.hpp>

// Engine wraps Game and shows UI to edit state of Game 
// On Game build only Game remains
// Nodes, Memory etc. with the flow

struct AppContext {
    SDL_Window* window;
    Renderer* renderer;
    AudioEngine* audioEngine;
    PhysicsEngine* physicsEngine;
    Engine* engine;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
};

SDL_AppResult SDL_Fail(){
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        return SDL_Fail();
    }
    
    if (not TTF_Init()) {
        return SDL_Fail();
    }
    
    SDL_Window* window = SDL_CreateWindow("Window", 352, 430, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (not window){
        return SDL_Fail();
    }
    
    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth){
            SDL_Log("This is a highdpi environment.");
        }
    }

    Renderer* renderer = new Renderer(window);
    AudioEngine* audioEngine = new AudioEngine();
    PhysicsEngine* physicsEngine = new PhysicsEngine();
    Engine* engine = new Engine();
    // set up the application data
    *appstate = new AppContext{
        .window = window,
        .renderer = renderer,
        .audioEngine = audioEngine,
        .physicsEngine = physicsEngine,
        .engine = engine,
    };
    
    SDL_Log("Application started successfully!");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) {
    auto* app = (AppContext*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto* app = (AppContext*)appstate;
    return app->app_quit;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    auto* app = (AppContext*)appstate;
    if (app) {
        delete app->renderer;
        delete app->audioEngine;
        delete app->physicsEngine;
        delete app->engine;
        SDL_DestroyWindow(app->window);
        delete app;
    }
    TTF_Quit();

    SDL_Log("Application quit successfully!");
    SDL_Quit();
}

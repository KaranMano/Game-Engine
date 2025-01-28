#include <renderer/renderer.hpp>

Renderer::Renderer(SDL_Window *_window)
    : m_window(_window)
{
    //m_GPUDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
    //SDL_assert(SDL_ClaimWindowForGPUDevice(m_GPUDevice, m_window));
}

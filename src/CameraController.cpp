#include "CameraController.hpp"

#include <SDL3/SDL_keyboard.h>
#include <iostream>

constexpr float DEFAULT_SPEED = 0.15;

CameraController::CameraController(Camera *camera)
    : m_camera(camera), m_speed(DEFAULT_SPEED)
{
}

void CameraController::SetSpeed(float speed)
{
    m_speed = speed;
}

void CameraController::UpdateRotation(float xrel, float yrel, float mouseSensitivity)
{
    m_camera->Rotate(xrel * mouseSensitivity, -yrel * mouseSensitivity);
}

void CameraController::Update(float delta)
{
    const bool* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_W]) {
        m_camera->Move(m_camera->GetFront() * m_speed);
    }
    else if(state[SDL_SCANCODE_S]) {
        m_camera->Move(-m_camera->GetFront() * m_speed);
    }

    if(state[SDL_SCANCODE_A]) {
        m_camera->Move(-m_camera->GetRight() * m_speed);
    }
    else if(state[SDL_SCANCODE_D]) {
        m_camera->Move(m_camera->GetRight() * m_speed);
    }

    if(state[SDL_SCANCODE_SPACE]) {
        m_camera->Move(m_camera->GetUp() * m_speed);
    }
    else if(state[SDL_SCANCODE_LSHIFT]) {
        m_camera->Move(-m_camera->GetUp() * m_speed);
    }
}

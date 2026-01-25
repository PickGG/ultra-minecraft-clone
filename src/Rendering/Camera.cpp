#include "Camera.hpp"

#include <SDL3/SDL_video.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

Camera::Camera(SDL_Window *window)
    : m_window(window), m_yaw(-90.0), m_pitch(0.0)
{
    UpdateFrontVector();
}

void Camera::SetPosition(glm::vec3 position)
{
    m_position = position;
}

void Camera::Move(glm::vec3 offset)
{
    m_position += offset;
}

void Camera::Rotate(float yaw, float pitch)
{
    m_yaw += yaw;
    m_pitch += pitch;

    UpdateFrontVector();
}

glm::mat4x4 Camera::CalcMatrix() const
{
    int width, height;
    SDL_GetWindowSizeInPixels(m_window, &width, &height);

    glm::mat4x4 projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    glm::mat4x4 view = glm::lookAt(m_position, m_position + m_front, UP);

    return projection * view;
}

glm::vec3 Camera::GetFront() const
{
    return m_front;
}

glm::vec3 Camera::GetRight() const
{
    return glm::cross(m_front, UP);
}

glm::vec3 Camera::GetUp() const
{
    return UP;
}

void Camera::UpdateFrontVector()
{
    m_front.x = SDL_cos(glm::radians(m_pitch)) * SDL_cos(glm::radians(m_yaw));
    m_front.y = SDL_sin(glm::radians(m_pitch));
    m_front.z = SDL_cos(glm::radians(m_pitch)) * SDL_sin(glm::radians(m_yaw));
    m_front = glm::normalize(m_front);
}

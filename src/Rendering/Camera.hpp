#pragma once

#include <glm/mat4x4.hpp>

struct SDL_Window;

class Camera
{
    SDL_Window* m_window;
    glm::vec3 m_position;
    glm::vec3 m_front;
    float m_yaw, m_pitch;
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
public:
    Camera(SDL_Window* window);
    void SetPosition(glm::vec3 position);
    void Move(glm::vec3 offset);
    void Rotate(float yaw, float pitch);
    // Returns Projection * View
    glm::mat4x4 CalcMatrix() const;
    glm::vec3 GetFront() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
private:
    void UpdateFrontVector();
};
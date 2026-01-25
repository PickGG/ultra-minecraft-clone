#pragma once

#include "Rendering/Camera.hpp"

class CameraController
{
    Camera* m_camera;
    float m_speed;
public:
    CameraController(Camera* camera);
    void SetSpeed(float speed);
    void Update(float delta);
    void UpdateRotation(float xrel, float yrel, float mouseSensitivity);
};

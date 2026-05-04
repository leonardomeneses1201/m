#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

class Camera {
public:
    glm::vec3 pos;
    float yaw   = -90.0f;   // degrees, -90 = looking into -Z
    float pitch =   0.0f;
    float fov   =  70.0f;
    float speed =   8.0f;
    float sensitivity = 0.12f;

    glm::vec3 front = { 0, 0, -1 };
    glm::vec3 up    = { 0, 1,  0 };
    glm::vec3 right = { 1, 0,  0 };

    explicit Camera(glm::vec3 startPos = { 0.f, 80.f, 0.f }) : pos(startPos) {
        updateVectors();
    }

    void processMouseMovement(float dx, float dy) {
        yaw   += dx * sensitivity;
        pitch  = std::clamp(pitch - dy * sensitivity, -89.0f, 89.0f);
        updateVectors();
    }

    glm::mat4 getView() const {
        return glm::lookAt(pos, pos + front, glm::vec3(0, 1, 0));
    }

    glm::mat4 getProjection(float aspect) const {
        return glm::perspective(glm::radians(fov), aspect, 0.05f, 600.0f);
    }

private:
    void updateVectors() {
        float yr = glm::radians(yaw);
        float pr = glm::radians(pitch);
        front.x = std::cos(yr) * std::cos(pr);
        front.y = std::sin(pr);
        front.z = std::sin(yr) * std::cos(pr);
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, { 0, 1, 0 }));
        up    = glm::normalize(glm::cross(right, front));
    }
};

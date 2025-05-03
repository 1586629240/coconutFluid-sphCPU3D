#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum camMove {FORWARD,BACKWARD,LEFT,RIGHT};

constexpr float YAW   = -90.0f;
constexpr float ZOOM  =  45.0f;
constexpr float PITCH =   0.0f;
constexpr float SPEED =   0.5f;
constexpr float SENSITIVITY = 0.1f;

class Camera
{
private:
    glm::vec3 camRight, worldUp;
    glm::vec3 camPos, camFront, camUp;

    float yaw, pitch;
    float zoom = ZOOM;
    float moveSpeed = SPEED;
    float mouseSensitivity = SENSITIVITY;

private:
    void updateCamVec()
    {
        glm::vec3 front;
        front.y = sin(glm::radians(pitch));
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camFront = glm::normalize(front);
        camRight = glm::normalize(glm::cross(camFront, worldUp));
        camUp = glm::normalize(glm::cross(camRight, camFront));
    }

public:
    Camera(
        glm::vec3 camPos_ = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 camUp_ = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw_ = YAW, float pitch_ = PITCH) : camFront(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        worldUp = camUp_;
        camPos = camPos_;
        yaw = yaw_; pitch = pitch_;
        updateCamVec();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(camPos, camPos + camFront, camUp);
    }

    void processKeyDown(camMove direction)
    {
        switch (direction)
        {
        case LEFT:camPos -= camRight * moveSpeed; break;
        case RIGHT:camPos += camRight * moveSpeed; break;
        case FORWARD:camPos += camFront * moveSpeed; break;
        case BACKWARD:camPos -= camFront * moveSpeed; break;
        }
    }

    void processMouseMove(float offsetX, float offsetY, GLboolean constrain = true)
    {
        yaw += (offsetX * mouseSensitivity);
        pitch += (offsetY * mouseSensitivity);

        if (constrain)
            pitch = fmin(89., pitch),
            pitch = fmax(-89., pitch);

        updateCamVec();
    }
    void processMouseScroll(float offset)
    {
        zoom -= offset*0.01;
        zoom = fmin(zoom, 45.);
        zoom = fmax(zoom, 1.);
    }
    float getFov() { return zoom; }
    glm::vec3 getPos() { return camPos; }
    glm::vec3 getFront() { return camFront; }

	glm::mat4 getInvViewMatrix()
	{
		return glm::inverse(GetViewMatrix());
	}

	glm::mat4 getInvProjectMatrix()
	{
		return glm::inverse(glm::perspective(zoom, (float)1920. / (float)1080., 0.1f, 1000.0f));
	}

	glm::mat4 getProjectionMatrix()
	{
		return glm::perspective(zoom, (float)1920. / (float)1080., 0.1f, 1000.0f);
	}
};
#endif
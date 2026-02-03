#include "Camera.h"

const float smoothFactor = 10.f;
sf::Vector2f currentCameraPos;

void Camera::setupCamera(sf::Vector2f camPosition) {
    currentCameraPos = camPosition;
    viewCam.setSize({ 400, 300 });
    viewCam.setCenter(currentCameraPos);
}

void Camera::updateCamera(sf::Vector2f newCamPos, float dt) {
    currentCameraPos += (newCamPos - currentCameraPos) * smoothFactor * dt;
    viewCam.setCenter(currentCameraPos);
}
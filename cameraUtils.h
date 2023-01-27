#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>

#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H



void orbitMouseHandle(float orbitRadius, glm::vec3 &cameraFront, glm::vec3 &cameraPos, float &cameraYaw, float &cameraPitch ){
 /// MAKE FROM HERE DOWN ITS OWN FUNCTION TO BE CALLED IN THE MOUSE HANDLER IN THE MAIN FILE. THE CURSORCALLBACK THING DOESN'T LIKE TAKING A MEMBER FUNCTION.
    cameraFront.x = -1* cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront.y = -1* sin(glm::radians(cameraPitch));
    cameraFront.z = -1* sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraFront);

    cameraPos.x = orbitRadius* cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraPos.y = orbitRadius* sin(glm::radians(cameraPitch));
    cameraPos.z = orbitRadius* sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    
}


void FPSCameraHandle(glm::vec3 &cameraFront, glm::vec3 &cameraPos, float &cameraYaw, float &cameraPitch ){

    cameraFront.x =  cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront.y = sin(glm::radians(cameraPitch));
    cameraFront.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraFront);

}


#endif
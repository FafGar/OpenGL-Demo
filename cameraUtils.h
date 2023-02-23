#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>

#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H



void orbitMouseHandle(float orbitRadius, glm::vec3 &cameraPos, float &cameraYaw, float &cameraPitch , glm::vec3 &centerPos){

    cameraPos.x = centerPos.x+ orbitRadius* cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraPos.y = centerPos.y+ orbitRadius* sin(glm::radians(cameraPitch));
    cameraPos.z = centerPos.z+ orbitRadius* sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    
}

// void orbitMouseHandle(float orbitRadius, glm::vec3 &cameraFront, glm::vec3 &cameraPos, float &cameraYaw, float &cameraPitch ){
//     if(cameraPitch){
//         cameraFront.y += -1* sin(glm::radians(cameraPitch));
//         cameraPos.y += orbitRadius* sin(glm::radians(cameraPitch));
//     }
//     if(cameraYaw){
//         cameraFront.x += -1* cos(glm::radians(cameraYaw));
//         cameraFront.z += -1* sin(glm::radians(cameraYaw));
//         cameraPos.x += orbitRadius* cos(glm::radians(cameraYaw));
//         cameraPos.z += orbitRadius* sin(glm::radians(cameraYaw));
//     }

//     // cameraFront = glm::normalize(cameraFront);


    
// }


void FPSCameraHandle(glm::vec3 &cameraFront, glm::vec3 &cameraPos, float &cameraYaw, float &cameraPitch ){

    cameraFront.x =  cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront.y = sin(glm::radians(cameraPitch));
    cameraFront.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraFront);

}


#endif
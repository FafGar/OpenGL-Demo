#ifndef FPS_CAMERA
#define FPS_CAMERA

#include <glm.hpp>


class FPScamera
{
private:
    
    glm::vec3 cameraPos = glm::vec3(0.f,0.f,0.f); //camera starting location
    glm::vec3 cameraFront = glm::vec3(0.f,0.f,1.f); //camera starts pointing here
    glm::vec3 upVec = glm::vec3(0.f, 1.f, 0.f); // world space up vector
     
    glm::vec3 cameraDirection;// positive z axis for the camera
    glm::vec3 cameraRight;//positive x vector for camera
    glm::vec3 cameraUp;//positive y vector for camera

    float cameraPitch = 0.f;
    float cameraYaw = -90.f;
    float lastX = 400;
    float lastY = 300;
    // float mouseSensitivity;
    bool mouseIn = true;

public:
    FPScamera(glm::vec3 position, glm::vec3 facingTowards);
    ~FPScamera();

    void handleMouse(GLFWwindow* window, double xPos, double yPos, float sensitivity);
    glm::mat4 getView();
};

FPScamera::FPScamera( glm::vec3 position, glm::vec3 facingTowards)
{
    cameraPos = position;
    cameraFront = facingTowards;
}

FPScamera::~FPScamera()
{
}



glm::mat4 FPScamera::getView(){
    return glm::lookAt(cameraPos, cameraPos+cameraFront, upVec);
}


void FPScamera::handleMouse(GLFWwindow* window, double xPos, double yPos, float sensitivity){
    if (mouseIn)
    {
        lastX = xPos;
        lastY = yPos;
        mouseIn = false;
    }
    
    float xOffset = xPos-lastX;
    float yOffset = lastY-yPos;
    lastX = xPos;
    lastY = yPos;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    cameraPitch += yOffset;
    cameraYaw += xOffset; 

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    cameraFront.x =  cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront.y = sin(glm::radians(cameraPitch));
    cameraFront.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraFront);

    cameraDirection = glm::normalize(cameraPos-cameraFront);
    cameraRight = glm::normalize(glm::cross(cameraDirection,upVec));
    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraFront));
}

#endif
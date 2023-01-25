#ifndef CIRCLE_CAMERA
#define CIRCLE_CAMERA

#include <glm.hpp>

class circleCamera
{
private:
    glm::vec3 cameraPos = glm::vec3(0.f,0.f,0.f); //camera starting location
    glm::vec3 cameraFront = glm::vec3(0.f,0.f,1.f); //camera starts pointing here
    glm::vec3 upVec = glm::vec3(0.f, 1.f, 0.f); // world space up vector
     
    glm::vec3 cameraDirection;// positive z axis for the camera
    glm::vec3 cameraRight;//positive x vector for camera
    glm::vec3 cameraUp;//positive y vector for camera

    //mouse input handling for camera
    float cameraPitch = 0.f;
    float cameraYaw = -90.f;
    float lastX = 400;
    float lastY = 300;
    const float mouseSensitivity = 0.1;
    bool mouseIn = true;

    float orbitRadius = 3;
public:
    circleCamera(float orbitRadius);
    ~circleCamera();

    void handleMouse(GLFWwindow* window, double xPos, double yPos);
    glm::mat4 getView();
    void use(GLFWwindow* useWindow);
};

circleCamera::circleCamera(float radius){
    orbitRadius = radius;
}

circleCamera::~circleCamera(){
}

glm::mat4 circleCamera::getView(){
    return glm::lookAt(cameraPos, cameraPos+cameraFront, upVec);
}

void circleCamera::use(GLFWwindow* useWindow){
    glfwSetCursorPosCallback(useWindow, handleMouse); ///
}

void circleCamera::handleMouse(GLFWwindow* window, double xPos, double yPos){
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

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    cameraPitch += yOffset;
    cameraYaw += xOffset; 

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;
 /// MAKE FROM HERE DOWN ITS OWN FUNCTION TO BE CALLED IN THE MOUSE HANDLER IN THE MAIN FILE. THE CURSORCALLBACK THING DOESN'T LIKE TAKING A MEMBER FUNCTION.
    cameraFront.x = -1* cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront.y = -1* sin(glm::radians(cameraPitch));
    cameraFront.z = -1* sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraFront);

    cameraPos.x = orbitRadius* cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraPos.y = orbitRadius* sin(glm::radians(cameraPitch));
    cameraPos.z = orbitRadius* sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

    cameraDirection = glm::normalize(cameraPos-cameraFront);
    cameraRight = glm::normalize(glm::cross(cameraDirection,upVec));
    cameraUp = glm::normalize(glm::cross(cameraDirection, cameraFront));
    
}
#endif
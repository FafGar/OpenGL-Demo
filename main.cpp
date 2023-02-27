/*
Author: Asher Dykstra

WARNING TO ALL THOSE WHO ENTER THIS FILE: IT'S A MESS AND I HAVE NOT HAD THE OPPORTUNITY TO CLEAN IT UP AS OF YET.


*/


#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include "cameraUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//TODO: Make hte camera follow the cube as it moves. Seems to be the result of how the orbit camera is handled. Simplest solution would seem to just be give it a point to orbit around.
// update aFTER A COUPLE HOURS. still trying to give it a point to orbit around.


void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //NOTE: the fact that the ints are passed by value instead of reference is evidently important.
    glViewport(0,0, width, height); //setting window size for OpenGL. This is for coordinate reasons mostly. Note: can be set small than actual window if you want to have other things outside the OpenGL render viewport
}

glm::vec3 cubePos = glm::vec3(0.f,0.f,0.f);

glm::vec3 cameraPos = glm::vec3(0.f,0.f,-3.f); //camera starting location
glm::vec3 upVec = glm::vec3(0.f, 1.f, 0.f); // world space up vector

glm::vec3 cameraDirection;// positive z axis for the camera
glm::vec3 cameraRight;//positive x vector for camera
glm::vec3 cameraUp;//positive y vector for camera

float cameraPitch = 0.f;
float cameraYaw = -90.f;
float lastX = 400;
float lastY = 300;
float mouseSensitivity = 0.1;
bool mouseIn = true;

void mouse_callback(GLFWwindow* window, double xPos, double yPos){
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

    orbitMouseHandle(3, cameraPos, cameraYaw,  cameraPitch, cubePos);    
    
    cameraDirection = glm::normalize(cubePos-cameraPos);
    cameraRight = glm::normalize(glm::cross(cameraDirection,upVec));
    cameraUp = glm::normalize(glm::cross(cameraDirection, cubePos-cameraPos));
}

void moveCube(glm::mat4 &model, float deltatime, glm::mat4 &view, glm::vec3 &followPos){
    glm::vec3 movement = 5.f*deltatime*cameraDirection;
    movement = glm::vec3(movement.x, 0, movement.z);
    model = glm::translate(model, movement);
    followPos+=movement;
    cameraPos += movement;
    cout << "MOVE" << endl;

    if (followPos.x > 4.5f){
        model = glm::translate(model, -glm::vec3(movement.x,0,0));
        followPos.x-=movement.x;
        cameraPos.x -= movement.x;
    } else if (followPos.x < -4.5f){
        model = glm::translate(model, -glm::vec3(movement.x,0,0));
        followPos.x-=movement.x;
        cameraPos.x -= movement.x;
    }
    if (followPos.z > 4.5f){
        model = glm::translate(model, -glm::vec3(0,0,movement.z));
        followPos.z-=movement.z;
        cameraPos.z -= movement.z;
    } else if (followPos.z < -4.5f){
        model = glm::translate(model, -glm::vec3(0,0,movement.z));
        followPos.z-=movement.z;
        cameraPos.z -= movement.z;
    }
}


bool textureShow = false;
void userInput(GLFWwindow* window, glm::mat4 &model, float deltatime, glm::mat4 &view, glm::vec3& cubePos){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); //close window if esc is pressed NOTE: the getKey func can also return GLFW_RELEASE
    // if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) glClear(GL_COLOR_BUFFER_BIT); // clear window to color if "C" is pressed
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//controlling if things are rendered wireframe or not
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {textureShow = true;}
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {textureShow = false;}
    if(glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {moveCube(model, deltatime, view, cubePos);}
}


int main() {

    glfwInit(); //initialize GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Tell GLFW to work with OpenGL version 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  //Tell GLFW to work with OpenGL version x.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW to work with openGL Core instead of the other one with unnecessary backwards compatibility things

    GLFWwindow* window = glfwCreateWindow(1800, 900, "Test Window", NULL, NULL);//Create window. In real application might want to check that pointer is not NULL before continuing

    glfwMakeContextCurrent(window); // makes "window" the main context of the current thread. My guess is that this will have to do with buffers/updating the frame later.
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ 
        //function inside the if statement is GLAD initializing the function pointers for the given OS. the result of glfwGetProcAddress is cast to something it can understand
        std::cout << "GLAD no workie :[" << std::endl;
        return -1;
    }    

   // glViewport(0,0, 800,600);         This line left in for documentation and learning purposes. It only need be called in the function defined at the top of the document

  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Using function above to tell window what to do when resized. GLFW calls whatever function passed with shown arguments in that order

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glClearColor(0.5f, .5f, 0.5f, 1.0f); // Sets color to clear the screen to when necessary. This produces a nice red, it is unclear what the last number does


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,    1.f, 1.f, 1.f,
        0.5f, -0.5f, -0.5f,     1.f, 1.f, 1.f,
        0.5f,  0.5f, -0.5f,     1.f, 1.f, 1.f,
        0.5f,  0.5f, -0.5f,     0.f, 0.f, 0.f,
        -0.5f,  0.5f, -0.5f,    0.f, 0.f, 0.f,
        -0.5f, -0.5f, -0.5f,    0.f, 0.f, 0.f,

        -0.5f, -0.5f,  0.5f,    0.5f, 0.1f, 0.2f,
        0.5f, -0.5f,  0.5f,     0.5f, 0.1f, 0.2f,
        0.5f,  0.5f,  0.5f,     0.5f, 0.1f, 0.2f,
        0.5f,  0.5f,  0.5f,     0.8f, 0.5f, 0.1f,
        -0.5f,  0.5f,  0.5f,    0.8f, 0.5f, 0.1f,
        -0.5f, -0.5f,  0.5f,    0.8f, 0.5f, 0.1f,

        -0.5f,  0.5f,  0.5f,    1.f, 0.8f, 0.8f,
        -0.5f,  0.5f, -0.5f,    1.f, 0.8f, 0.8f,
        -0.5f, -0.5f, -0.5f,    1.f, 0.8f, 0.8f,
        -0.5f, -0.5f, -0.5f,    0.2f, 1.f, 0.9f,
        -0.5f, -0.5f,  0.5f,    0.2f, 1.f, 0.9f,
        -0.5f,  0.5f,  0.5f,    0.2f, 1.f, 0.9f,

        0.5f,  0.5f,  0.5f,     0.6f, 0.3f, 1.f,
        0.5f,  0.5f, -0.5f,     0.6f, 0.3f, 1.f,
        0.5f, -0.5f, -0.5f,     0.6f, 0.3f, 1.f,
        0.5f, -0.5f, -0.5f,     0.5f, 0.2f, 0.7f,
        0.5f, -0.5f,  0.5f,     0.5f, 0.2f, 0.7f,
        0.5f,  0.5f,  0.5f,     0.5f, 0.2f, 0.7f,

        -0.5f, -0.5f, -0.5f,    0.1f, 0.4f, 0.3f,
        0.5f, -0.5f, -0.5f,     0.1f, 0.4f, 0.3f,
        0.5f, -0.5f,  0.5f,     0.1f, 0.4f, 0.3f,
        0.5f, -0.5f,  0.5f,     0.9f, 0.7f, 0.9f,
        -0.5f, -0.5f,  0.5f,    0.9f, 0.7f, 0.9f,
        -0.5f, -0.5f, -0.5f,    0.9f, 0.7f, 0.9f,

        -0.5f,  0.5f, -0.5f,    0.1f, 0.8f, 0.5f,
        0.5f,  0.5f, -0.5f,     0.1f, 0.8f, 0.5f,
        0.5f,  0.5f,  0.5f,     0.1f, 0.8f, 0.5f,
        0.5f,  0.5f,  0.5f,     0.9f, 0.4f, 0.2f,
        -0.5f,  0.5f,  0.5f,    0.9f, 0.4f, 0.2f,
        -0.5f,  0.5f, -0.5f,    0.9f, 0.4f, 0.2f,
    };

    float texVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.f, 0.f,
        0.5f, -0.5f, -0.5f,   0.f,1.f,
        0.5f,  0.5f, -0.5f,     1.f,1.f,
        0.5f,  0.5f, -0.5f,     1.f,1.f,
        -0.5f,  0.5f, -0.5f,    1.f,0.f,
        -0.5f, -0.5f, -0.5f,  0.f, 0.f,

        -0.5f, -0.5f,  0.5f, 0.f, 0.f,
        0.5f, -0.5f,  0.5f, 0.f,1.f,
        0.5f,  0.5f,  0.5f, 1.f,1.f,
        0.5f,  0.5f,  0.5f,  1.f,1.f,
        -0.5f,  0.5f,  0.5f,   1.f,0.f,
        -0.5f, -0.5f,  0.5f,  0.f, 0.f,

        -0.5f,  0.5f,  0.5f,  0.f, 0.f,
        -0.5f,  0.5f, -0.5f,  0.f,1.f,
        -0.5f, -0.5f, -0.5f, 1.f,1.f,
        -0.5f, -0.5f, -0.5f, 1.f,1.f,
        -0.5f, -0.5f,  0.5f,  1.f,0.f,
        -0.5f,  0.5f,  0.5f, 0.f, 0.f,

        0.5f,  0.5f,  0.5f, 0.f, 0.f,
        0.5f,  0.5f, -0.5f, 0.f,1.f,
        0.5f, -0.5f, -0.5f, 1.f,1.f,
        0.5f, -0.5f, -0.5f, 1.f,1.f,
        0.5f, -0.5f,  0.5f, 1.f,0.f,
        0.5f,  0.5f,  0.5f,  0.f, 0.f,

        -0.5f, -0.5f, -0.5f, 0.f, 0.f,
        0.5f, -0.5f, -0.5f, 0.f,1.f,
        0.5f, -0.5f,  0.5f,  1.f,1.f,
        0.5f, -0.5f,  0.5f,  1.f,1.f,
        -0.5f, -0.5f,  0.5f, 1.f,0.f,
        -0.5f, -0.5f, -0.5f, 0.f, 0.f,

        -0.5f,  0.5f, -0.5f, 0.f, 0.f,
        0.5f,  0.5f, -0.5f, 0.f,1.f,
        0.5f,  0.5f,  0.5f,  1.f,1.f,
        0.5f,  0.5f,  0.5f,  1.f,1.f,
        -0.5f,  0.5f,  0.5f, 1.f,0.f,
        -0.5f,  0.5f, -0.5f, 0.f, 0.f
    };

    float floorVertices[] = {
        -5.f, -0.51f, -5.f,      1.f, 0.f, 0.f, 
        5.f,  -0.51f, -5.f,      0.f, 0.f, 0.8f, 
        5.f, -0.51f, 5.f,        0.f, 0.f, 0.8f,
        5.f, -0.51f, 5.f,        0.f, 0.f, 0.8f,
        -5.f,  -0.51f, 5.f,      0.f, 0.f, 0.8f,
        -5.f,  -0.51f, -5.f,     1.f, 0.f, 0.f,
    };
    
    // unsigned int indices[] = { //orders of indices of vertexes (That makes sense, right?) to draw in order to form a rectangle without excess vertex definitions
    //     0, 1, 3,   // first triangle
    //     1, 2, 3    // second triangle
    // }  ; 

    unsigned int EBO;
    glGenBuffers(1, &EBO); //Creating an Element Buffer object

    unsigned int EBO1;
    glGenBuffers(1, &EBO1);
    

    unsigned int VBO;
    glGenBuffers(1, &VBO); //an OpenGl object is created, the ID of which is assigned to VBO. The first number is # of ids to generate, so the second arg could/should be an array in other circumstances.
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);

    unsigned int VBOFloor;
    glGenBuffers(1, &VBOFloor);

    

    shader shader1("../shaders/genericVert.glsl", "../shaders/genericFrag.glsl");
    shader shader2("../shaders/textureVert.glsl", "../shaders/textureFrag.glsl");
    // shader shader2("shaders/genericVert.glsl", "shaders/genericFrag2.glsl");
    
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);// creating a Vertex Array Object the same way we created the Vertex Buffer Object.
    // The VAO really just serves as a way to save and reload all of the vertex array settings we established above. This is an example of OpenGL being a state machine

    glBindVertexArray(VAO); //Set state so that settings are usable later.                  // This could be called again in render loop to switch vertex array setting states

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds the buffer VBO points at to being the array/vertex buffer target.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //
    //           ^data sent to    ^data size (bytes) ^data    ^How GPU handles data; this option means data is sent once and used many times

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // This binding is one of the many things that is saved in a VAO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0); // telling the GPU how to interpret the data we sent to the shaders: position data for each vertex is first in line at index 0, positions are in vec3, the individual values are in
    //floats, the values need not be normalized in this case, the width of each vertex's data is just 3 floats, and the last one is the offset from the beginning where the position data starts in the buffer.
    glEnableVertexAttribArray(0);// Enables the 0 indexed attribute of a vertex. In this case, position.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float))); 
    glEnableVertexAttribArray(1);


    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);

    glBindVertexArray(VAO1);\

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(texVertices), texVertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0); //evidently this and the next line must be specified for each Vertex Array Object.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float))); 
    glEnableVertexAttribArray(1);

    
    glEnable(GL_TEXTURE_2D);
    int width, height, nrChannels;
    unsigned char *textureData = stbi_load("../textures/blacksmith.png", &width, &height, &nrChannels,0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //texture handling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    
    if(textureData){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        cout << "failed to load texture";
        return -1;
    }

    stbi_image_free(textureData);

    glEnable(GL_DEPTH_TEST);



    unsigned int VAOFloor;
    glGenVertexArrays(1,&VAOFloor);// creating a Vertex Array Object the same way we created the Vertex Buffer Object.
    // The VAO really just serves as a way to save and reload all of the vertex array settings we established above. This is an example of OpenGL being a state machine

    glBindVertexArray(VAOFloor); //Set state so that settings are usable later.                  // This could be called again in render loop to switch vertex array setting states

    glBindBuffer(GL_ARRAY_BUFFER, VBOFloor); //binds the buffer VBO points at to being the array/vertex buffer target.

    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW); //
    //           ^data sent to    ^data size (bytes)        ^data        ^How GPU handles data; this option means data is sent once and used many times

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0); // telling the GPU how to interpret the data we sent to the shaders: position data for each vertex is first in line at index 0, positions are in vec3, the individual values are in
    //floats, the values need not be normalized in this case, the width of each vertex's data is just 3 floats, and the last one is the offset from the beginning where the position data starts in the buffer.
    glEnableVertexAttribArray(0);// Enables the 0 indexed attribute of a vertex. In this case, position.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float))); 
    glEnableVertexAttribArray(1);


    glm::mat4 model = glm::mat4(1.f);
    // model = glm::rotate(model,(float)glfwGetTime()*glm::radians(15.f), glm::vec3(.5f,1.f, 0.f));

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 1800.f/900.f, 0.1f, 100.f );

    glm::mat4 view;

    shader2.use();
    glUniform1i(glGetUniformLocation(shader2.ID, "inTexture"), 0);
   
 
    //Deltatime handling
    float deltaTime = 0.0;
    float lastFrame = 0.0;
    float currentFrame;

    

    while(!glfwWindowShouldClose(window)){ //render loop        The function just checks if the window has been told to close or not
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        view = glm::lookAt(cameraPos, cubePos, upVec);
        
        // shader1.

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        userInput(window, model, deltaTime, view, cubePos);

        // glUseProgram(shaderProg);
        if(textureShow){
            shader2.use();
            glBindVertexArray(VAO1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            int modelLocation = glGetUniformLocation(shader2.ID, "model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            int viewLocation = glGetUniformLocation(shader2.ID, "view");
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE,glm::value_ptr(view));

            int projectionLocation = glGetUniformLocation(shader2.ID, "projection");
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        }else{
            shader1.use();
            glBindVertexArray(VAO);

            int modelLocation = glGetUniformLocation(shader1.ID, "model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            int viewLocation = glGetUniformLocation(shader1.ID, "view");
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE,glm::value_ptr(view));

            int projectionLocation = glGetUniformLocation(shader1.ID, "projection");
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        }

        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
         // draws triangles based on items in the currently bound element buffer. 
        // 1: What to draw      2: how many elements(vertices here)     3: what data type the indices are       4: offset from start of index data

        shader1.use();
        glBindVertexArray(VAOFloor);
        int modelLocation = glGetUniformLocation(shader1.ID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window); //swaps the color buffer(contains color values for every pixel)
        glfwPollEvents(); // checks for input events, updates window state and can call callback funcs.
    }

    glfwTerminate(); //Being a good little programmer, deallocating resources cleanly.
    return 0;
}
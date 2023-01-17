#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //NOTE: the fact that the ints are passed by value instead of reference is evidently important.
    glViewport(0,0, width, height); //setting window size for OpenGL. This is for coordinate reasons mostly. Note: can be set small than actual window if you want to have other things outside the OpenGL render viewport
}

void userInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true); //close window if esc is pressed NOTE: the getKey func can also return GLFW_RELEASE
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) glClear(GL_COLOR_BUFFER_BIT); // clear window to color if "C" is pressed
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//controlling if things are rendered wireframe or not
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
// const char* vertexShaderSource = "#version 460 core\n" // Apparently this is the simple/beginner way to write stuff in GLSL. It has to be compiled at runtime
//     "layout (location = 0) in vec3 aPos;\n"
//     "void main(){\n"
//     "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}\0";

// const char* fragShaderSource = "#version 460 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);}\0";

// const char* fragShaderSource1 = "#version 460 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{FragColor = vec4(0.0f, 0.5f, 1.f, 1.0f);}\0";

int main() {

    glfwInit(); //initialize GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //Tell GLFW to work with OpenGL version 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  //Tell GLFW to work with OpenGL version x.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Tell GLFW to work with openGL Core instead of the other one with unnecessary backwards compatibility things

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);//Create window. In real application might want to check that pointer is not NULL before continuing

    glfwMakeContextCurrent(window); // makes "window" the main context of the current thread. My guess is that this will have to do with buffers/updating the frame later.
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ 
        //function inside the if statement is GLAD initializing the function pointers for the given OS. the result of glfwGetProcAddress is cast to something it can understand
        std::cout << "GLAD no workie :[" << std::endl;
        return -1;
    }    

   // glViewport(0,0, 800,600);         This line left in for documentation and learning purposes. It only need be called in the function defined at the top of the document

  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Using function above to tell window what to do when resized. GLFW calls whatever function passed with shown arguments in that order

    glClearColor(0.808f, .09f, 0.7f, 1.0f); // Sets color to clear the screen to when necessary. This produces a nice red, it is unclear what the last number does


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f,  0.5f, 
        0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = { //orders of indices of vertexes (That makes sense, right?) to draw in order to form a rectangle without excess vertex definitions
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }  ; 

    unsigned int indices2[] = {
        0, 4, 3 //rooftop triangle
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO); //Creating an Element Buffer object

    unsigned int EBO1;
    glGenBuffers(1, &EBO1);
    

    unsigned int VBO;
    glGenBuffers(1, &VBO); //an OpenGl object is created, the ID of which is assigned to VBO. The first number is # of ids to generate, so the second arg could/should be an array in other circumstances.


    shader shader1("shaders/genericVert.glsl", "shaders/genericFrag.glsl");
    shader shader2("shaders/genericVert.glsl", "shaders/genericFrag2.glsl");
    
    unsigned int VAO;
    glGenVertexArrays(1,&VAO);// creating a Vertex Array Object the same way we created the Vertex Buffer Object.
    // The VAO really just serves as a way to save and reload all of the vertex array settings we established above. This is an example of OpenGL being a state machine

    glBindVertexArray(VAO); //Set state so that settings are usable later.                  // This could be called again in render loop to switch vertex array setting states

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //binds the buffer VBO points at to being the array/vertex buffer target.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //
    //           ^data sent to    ^data size (bytes) ^data    ^How GPU handles data; this option means data is sent once and used many times


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // This binding is one of the many things that is saved in a VAO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // telling the GPU how to interpret the data we sent to the shaders: position data for each vertex is first in line at index 0, positions are in vec3, the individual values are in
    //floats, the values need not be normalized in this case, the width of each vertex's data is just 3 floats, and the last one is the offset from the beginning where the position data starts in the buffer.
    glEnableVertexAttribArray(0);// Enables the 0 indexed attribute of a vertex. In this case, position.


    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);

    glBindVertexArray(VAO1);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); //evidently this and the next line must be specified for each Vertex Array Object.
    glEnableVertexAttribArray(0); 

    glEnable(GL_DEPTH_TEST);


    glm::mat4 model = glm::mat4(1.f);
    model = glm::rotate(model,(float)glfwGetTime()*glm::radians(15.f), glm::vec3(.5f,1.f, 0.f));

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view,glm::vec3(0.f,0.f,-3.f));

    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(glm::radians(45.f), 800.f/600.f, 0.1f, 100.f );

    // int modelLocation = glGetUniformLocation(shader1.ID, "model");
    // glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    // int viewLocation = glGetUniformLocation(shader1.ID, "view");
    // glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    // int projectionLocation = glGetUniformLocation(shader1.ID, "projection");
    // glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


    while(!glfwWindowShouldClose(window)){ //render loop        The function just checks if the window has been told to close or not
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        userInput(window);

        // glUseProgram(shaderProg);
        shader1.use();
        glBindVertexArray(VAO);


        model = glm::rotate(model,glm::radians(0.5f), glm::vec3(.5f,1.f, 0.f));
        int modelLocation = glGetUniformLocation(shader1.ID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        int viewLocation = glGetUniformLocation(shader1.ID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLocation = glGetUniformLocation(shader1.ID, "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, 36); // draws triangles based on items in the currently bound element buffer. 
        // 1: What to draw      2: how many elements(vertices here)     3: what data type the indices are       4: offset from start of index data


        // shader2.use();
        // glBindVertexArray(VAO1);
        
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); //swaps the color buffer(contains color values for every pixel)
        glfwPollEvents(); // checks for input events, updates window state and can call callback funcs.
    }

    glfwTerminate(); //Being a good little programmer, deallocating resources cleanly.
    return 0;
}

#ifndef SHADER_H
#define SHADER_H


#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class shader
{
public:
    unsigned int ID;
    shader(const string vertexP, const string fragP);
    ~shader();

    void use();
    void setInt(const string &name, int value);
    void setFloat(const string &name, float value);
    void setVec(const string &name, float value, float value2);// for later implementation 
    void setVec(const string &name, float value, float value2, float value3);// for later implementation 
    void setVec(const string &name, float value, float value2, float value3, float value4);// for later implementation 
};

shader::shader(const string vertexP, const string fragP)
{
    ifstream vertexF;                       //reading in shader code from supplied files
    vertexF.open(vertexP);
    if(!vertexF.is_open()) cout<< "Vertex File Read Failed" << endl;
    ifstream fragF;
    fragF.open(fragP);
    if(!fragF.is_open()) cout<< "Fragment File Read Failed" << endl;

    stringstream vertexS, fragS;
    vertexS << vertexF.rdbuf();             //Pushing text from files into a string stream
    fragS << fragF.rdbuf();

    fragF.close();                          //closing shader code files
    vertexF.close();


    string vertexTemp = vertexS.str();
    string fragTemp = fragS.str();

    const char* vertexCode = vertexTemp.c_str();         //converting the file text into the c strings we need
    const char* fragCode = fragTemp.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //creating GL object IDs for the shaders
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infolog[512];

    glShaderSource(vertexShader, 1, &vertexCode, NULL); //associating the shader code with the GL object and compiling for vertex
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // help with debugging
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        cout << "Vertex shader compilation failed" << endl << infolog << endl;
        cout << vertexCode << endl;
    }

    glShaderSource(fragShader, 1, &fragCode, NULL);     //associating the shader code with the GL object and compiling for fragment
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success); // help with debugging
    if(!success){
        glGetShaderInfoLog(fragShader, 512, NULL, infolog);
        cout << "Fragment shader compilation failed" << endl << infolog << endl;
    }

    ID = glCreateProgram();                 //providing the shader program's ID
    glAttachShader(ID, vertexShader);       //attaching shaders to the shader program
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);                      //linking all the things together

    glGetProgramiv(ID, GL_LINK_STATUS, &success); // help with debugging
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infolog);
        cout << "Program linking failed" << endl << infolog << endl;
    }


    glDeleteShader(vertexShader); // shader cleanup
    glDeleteShader(fragShader);
}

shader::~shader()
{
    glDeleteProgram(ID);
}

void shader::use(){
    glUseProgram(ID);
}

void shader::setInt(const string &name, int value){ //function used for setting an integer value to be used in various shaders within the shader program
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void shader::setFloat(const string &name, float value){ //function used for setting a floating point value to be used in various shaders within the shader program
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}


#endif


    //              ORIGINAL SHADER CODE (FOR REFERENCE)

    // unsigned int vertexShader; //similar to above. Presumably the norm with GL objects
    // vertexShader = glCreateShader(GL_VERTEX_SHADER); //different syntax than above, seems to be very similar usage.1
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //arg1:target, arg2:# of strings in code, arg3: actual code, arg4:??
    // glCompileShader(vertexShader); // the compiling at runtime mentioned earlier

    // int success; //checking if the vertex shader compiled correctly.
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if(!success){
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cout << "VERTEX SHADER COMPILATION FAILED  " << infoLog <<std::endl;
    // }

    // unsigned int fragShader; // same as vertex shader, but for the fragment shader.
    // fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    // glCompileShader(fragShader);

    // unsigned int fragShader1; // second shader for rooftop.
    // fragShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragShader1, 1, &fragShaderSource1, NULL);
    // glCompileShader(fragShader1);

    // glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success); //checking if frag shader compiled. No new variables are necessary as the program will only use them upon the first failed component.
    // if(!success){
    //     glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
    //     std::cout << "FRAGMENT SHADER COMPILATION FAILED  " << infoLog <<std::endl;
    // }

    // unsigned int shaderProg; //creating a gpu program object upon which to attach shaders
    // shaderProg = glCreateProgram();
    
    // glAttachShader(shaderProg, vertexShader);//attaching compiled shaders to program object
    // glAttachShader(shaderProg, fragShader);
    // glLinkProgram(shaderProg);// linking all provided shaders into one program so that they are used together, creating a usable program object

    
    

    // unsigned int shaderProg1;
    // shaderProg1 = glCreateProgram();
    // glAttachShader(shaderProg1, vertexShader);
    // glAttachShader(shaderProg1, fragShader1);
    // glLinkProgram(shaderProg1);

    // glDeleteShader(fragShader1);


    // glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
    // if(!success){
    //     glGetProgramInfoLog(shaderProg, 512, NULL, infoLog);
    //     std::cout << "SHADER LINKING FAILED  " << infoLog << std::endl;
    // }

    // glDeleteShader(vertexShader);
    // glDeleteShader(fragShader);// deleting shaders as they are no longer useful as individual pieces.

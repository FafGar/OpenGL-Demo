#version 460 core
in vec3 color;

out vec4 FragColor;

void main(){
    FragColor = vec4(1.0, 1.0, 1.0, 0.4) *vec4(color, 1.0);
    
}
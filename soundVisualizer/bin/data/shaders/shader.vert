#version 150
precision mediump float;
 
in vec3 normal;
in vec4 color;
in vec3 position;
 
out vec3 vNormal;
out vec4 vColor;
out vec3 vPosition;
 
void main() {
    vNormal = normal;
    vColor = color;
    vPosition = position;
}
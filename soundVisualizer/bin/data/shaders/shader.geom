#version 150
precision mediump float;

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

uniform mat4 modelViewProjectionMatrix;
uniform int height;

in vec3 vNormal[];
in vec4 vColor[];
in vec3 vPosition[];

out vec4 gsColor;

void main(){
    //•½–Ê
    for(int i=0; i<gl_in.length(); i++){
        gl_Position = modelViewProjectionMatrix * vec4(vPosition[i], 1.0);
        gsColor = vColor[i];
        EmitVertex();
    }
    EndPrimitive();
    
    //‰š“Ê–Ê
    vec3 offset;
    for(int i=0; i<gl_in.length(); i++){
        offset = vNormal[i] * height * vColor[i].r;
        gl_Position = modelViewProjectionMatrix * vec4(vPosition[i] + (offset.xyz), 1.0);
        gsColor = vColor[i];
        EmitVertex();
    }
    EndPrimitive();
}
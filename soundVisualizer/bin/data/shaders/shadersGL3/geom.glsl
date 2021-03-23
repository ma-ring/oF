#version 150
#extension GL_EXT_geometry_shader4 : enable

uniform int fft[500];
uniform vec3 rgb[500];

uniform mat4 modelViewProjectionMatrix;

in vec3 vPosition[];
in vec3 vNormal[];
in vec4 vColor[];

out vec4 gsColor;

vec3 constructNormal(vec3 n1, vec3 n2, vec3 n3){
	return  -  normalize(cross(n2-n1, n3-n1));
}

void main(void){

	vec3 offset = constructNormal(vPosition[0],vPosition[1],vPosition[2]) *  fft[gl_PrimitiveIDIn];
	vec4 color =  vec4(rgb[gl_PrimitiveIDIn], 8.0);

	for(int i = 0; i < vPosition.length() ; i++){
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[i]  + offset , 1.0);
		gsColor = color;
		EmitVertex();
	}
	EndPrimitive();


	for(int i = 0; i < vPosition.length() ; i++){
		int nextId = i+ 1;
		if(nextId > vPosition.length() -  1)  nextId = 0;
	
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[i]  + offset , 1.0);
		gsColor = color;
		EmitVertex();
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[i]   , 1.0);
		gsColor = vec4(1.0,1.0,1.0,1.0);
		EmitVertex();
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[nextId]   , 1.0);
		gsColor = vec4(1.0,1.0,1.0,1.0);
		EmitVertex();
		
		EndPrimitive();

		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[i]  + offset , 1.0);
		gsColor = color;
		EmitVertex();
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[nextId]   , 1.0);
		gsColor = vec4(1.0,1.0,1.0,1.0);
		EmitVertex();
		gl_Position = modelViewProjectionMatrix *  vec4(vPosition[nextId]  + offset , 1.0);
		gsColor = color;
		EmitVertex();
	
		
		EndPrimitive();

	}

}
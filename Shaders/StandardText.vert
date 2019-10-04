#version 330 core

layout(location = 0) in vec2 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform vec2 aspectRatio;


void main(){

	vec2 vertexPosition_h = vertexPosition_screenspace - vec2(400,300);
	// vec2 vertexPosition_h = vertexPosition_screenspace - aspectRatio;
	vertexPosition_h /= vec2(400,300);
	// vertexPosition_h /= aspectRatio;
	gl_Position =  vec4(vertexPosition_h, 0, 1);
	
	UV = vertexUV;
}


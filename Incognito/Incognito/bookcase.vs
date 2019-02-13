#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform vec3 offset;
uniform mat4 myMat;

out vec4 myColor;
out vec2 TexCoord;

void main(){
	gl_Position = myMat * vec4(position + offset, 1.0f);
	myColor = vec4(color + offset, 1.0f);
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
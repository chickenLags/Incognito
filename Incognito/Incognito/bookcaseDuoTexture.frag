#version 330 core

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

in vec4 myColor;
in vec2 TexCoord;

out vec4 color;

void main(){
	color =  mix( texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.1 );
	color = mix( color, myColor, 0.2);
}
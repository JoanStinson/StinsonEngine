#version 430 core

in vec2 uv0;

uniform sampler2D texture0;

out vec4 color;

void main() {
	color = texture2D(texture0, uv0);
}
#version 330 core

in vec3 color;
in vec2 texCoord;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
out vec4 fragColor;

void main() {
	fragColor = mix(texture(sampler1, texCoord), texture(sampler2, texCoord), 0.2) * vec4(color, 1.0f);
}

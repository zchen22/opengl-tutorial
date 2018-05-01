#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
in vec2 texCoord;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
out vec4 fragColor;

void main() {
	fragColor = vec4(lightColor * objectColor, 1.0);
	fragColor *= mix(texture(sampler1, texCoord), texture(sampler2, texCoord), 0.2);
}

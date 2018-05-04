#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform mat4 lightModel;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

out vec4 fragColor;

void main() {
	// Calculate ambient
	float ambientFactor = 0.1;
	vec3 ambient = ambientFactor * lightColor;
	// Calculate diffuse
	vec3 lightPos = vec3(lightModel * vec4(fragPos, 1.0));
	float diffuseFactor = max(dot(normalize(lightPos - fragPos), normalize(normal)), 0);
	vec3 diffuse = diffuseFactor * lightColor;
	// Calculate final color
	fragColor = vec4((ambient + diffuse) * objectColor, 1.0);
	fragColor *= mix(texture(sampler1, texCoord), texture(sampler2, texCoord), 0.2);
}

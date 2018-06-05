#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
	// Calculate ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	// Calculate diffuse
	vec3 normalNormalized = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 diffuse = max(dot(normalNormalized, lightDir), 0.0) * lightColor;
	// Calculate specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normalNormalized);
	vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32) * lightColor;  
	// Calculate final color
	fragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
	fragColor *= mix(texture(sampler1, texCoord), texture(sampler2, texCoord), 0.2);
}

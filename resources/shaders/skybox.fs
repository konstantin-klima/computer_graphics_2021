#version 330 core
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube cube;
uniform vec3 ambientLight;

void main()
{
    fragColor = texture(cube, texCoords) * vec4(ambientLight, 1);
}
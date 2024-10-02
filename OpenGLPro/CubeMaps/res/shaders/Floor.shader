#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture2;

void main()
{             
    FragColor = vec4(0.1f, 0.4f, 1.0f, 1.0f) * texture(texture2, TexCoords)*0.5f;
}
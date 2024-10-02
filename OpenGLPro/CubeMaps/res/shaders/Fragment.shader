#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture1;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 texture2 = texture(texture1, TexCoords).rgb;
    FragColor = vec4(texture(skybox, R).rgb * 1.0f + texture2*0.0f, 1.0f);
}
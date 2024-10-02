#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 fColor;
} vs_in;

void main()
{
    FragColor = vec4(vs_in.fColor, 1.0);
}  
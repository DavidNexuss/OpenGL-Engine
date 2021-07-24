#version 330 core
out vec3 color;

uniform vec3 surface;
void main()
{
    color = vec3(surface);
}

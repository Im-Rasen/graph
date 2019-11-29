#version 330 core

in vec3 TexCoord; // Вектор направления, таже представляющий трехмерную текстурную координату
uniform samplerCube cubemap;
out vec4 color;

void main()
{
    color = texture(cubemap, TexCoord);
}

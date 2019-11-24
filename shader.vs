#version 330 core

layout (location = 0) in vec3 position; // Позиция - с координатами в 0
layout (location = 1) in vec3 color; // Цвет - с координатами в 1
layout (location = 2) in vec2 texCoord; // Текстура - с координатами в 2

//"out vec4 vertexColor;" // Передача цвета во фрагментный шейдер
out vec3 ourColor;
out vec4 ourPosition;
out vec2 TexCoord;

uniform float shiftX;
uniform mat4 transform;

void main()
{
    ourPosition = transform * vec4(position[0], position[1], position[2], 1.0); //+shiftX
    gl_Position = ourPosition;
    ourColor = color; // Значение цвета от вершинных данных
    //"vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);"
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}

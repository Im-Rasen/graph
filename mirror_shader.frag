#version 330 core

out vec4 color;

in vec3 TexCoord; // Вектор направления, таже представляющий трехмерную текстурную координату
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPosition;
uniform samplerCube cubemap;


void main()
{
    //color = texture(cubemap, TexCoord);
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPosition);
    vec3 R = refract(I, normalize(Normal), ratio);
    //vec3 R = reflect(I, normalize(Normal));
    color = vec4(texture(cubemap, R).rgb, 1.0);
}

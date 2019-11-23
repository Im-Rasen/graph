#version 330 core

//"in vec4 vertexColor;"

//uniform vec4 ourColor;

in vec3 ourColor;
in vec4 ourPosition;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mixRate;

void main()
{
    //"color = vertexColor;\n"
    //color = ourColor;
    //color = ourPosition;
    //color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), mixRate);
}

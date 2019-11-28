#version 330 core
#define NR_POINT_LIGHTS 4

out vec4 color;

in vec2 TexCoord;

uniform sampler2D ourTexture;


void main()
{
    /*
    vec4 texColor = texture(ourTexture, TexCoord);
    if(texColor.a < 0.1)
        discard;
    color = texColor;
    */
    color = texture(ourTexture, TexCoord);
}

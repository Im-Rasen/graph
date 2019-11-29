#version 330 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    //color = texture(screenTexture, TexCoord);
    //color = vec4(vec3(1.0 - texture(screenTexture, TexCoord)), 1.0); // Инверсия цвета
    
    //Серый
    vec4 midColor = texture(screenTexture, TexCoord);
    float average = (0.2126 * midColor.r + 0.7152 * midColor.g + 0.0722 * midColor.b) / 3.0;
    color = vec4(average, average, average, 1.0);
     
}

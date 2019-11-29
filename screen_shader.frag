#version 330 core
out vec4 color;
  
in vec2 TexCoord;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0; // Подбор

void main()
{
    //color = texture(screenTexture, TexCoord);
    //color = vec4(vec3(1.0 - texture(screenTexture, TexCoord)), 1.0); // Инверсия цвета
    /*
    //Серый
    vec4 midColor = texture(screenTexture, TexCoord);
    float average = (0.2126 * midColor.r + 0.7152 * midColor.g + 0.0722 * midColor.b) / 3.0;
    color = vec4(average, average, average, 1.0);
     */
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    /* //Наркотический трип (повышение резкозти)
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    */
    /* //Размытие
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );
     */
    //Границы
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    color = vec4(col, 1.0);
     
}

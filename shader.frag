#version 330 core
#define NR_POINT_LIGHTS 4

out vec4 color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

float zNear = 0.1;
float zFar  = 100.0;
  
float LinearizeDepth(float depth)
{
    // преобразуем обратно в NDC
    float z = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}


void main()
{
    //color = texture(ourTexture, TexCoord);
    //color = vec4(vec3(gl_FragCoord.z), 1.0);
    float depth = LinearizeDepth(gl_FragCoord.z) / zFar;
    color = vec4(vec3(depth), 1.0);
}

#version 330 core
//"in vec4 vertexColor;"
//uniform vec4 ourColor;
//in vec3 ourColor;
//in vec4 ourPosition;
//in vec2 TexCoord;

out vec4 color;

in vec3 Normal;
in vec3 worldPosition;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPosition;


//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

//uniform float mixRate;



void main()
{
    //"color = vertexColor;\n"
    //color = ourColor;
    //color = ourPosition;
    //color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
    //color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), mixRate);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - worldPosition); // Вектор от источника к объекту
    float diffuseRate = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseRate * lightColor;
    
    vec3 trueColor = (ambient + diffuse) * objectColor;
    vec4 theColor = vec4(trueColor, 1.0);
    color = theColor;
}

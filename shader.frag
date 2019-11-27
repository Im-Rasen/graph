#version 330 core
//"in vec4 vertexColor;"
//uniform vec4 ourColor;
//in vec3 ourColor;
//in vec4 ourPosition;


struct Material {
    //vec3 ambient;
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
  
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


out vec4 color;

in vec3 Normal;
in vec3 worldPosition; // Позиция текущего фрагмента
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;


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
    
    //vec3 ambient = light.ambient * material.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - worldPosition); // Вектор от объекта к источнику
    float diffuseRate = max(dot(norm, lightDirection), 0.0);
    //vec3 diffuse = light.diffuse * diffuseRate * material.diffuse;
    vec3 diffuse = light.diffuse * diffuseRate * vec3(texture(material.diffuse, TexCoord));
    
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm); // Первый аргумент - от источника к объекту
    
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 trueColor = diffuse + specular; // + ambient
    vec4 theColor = vec4(trueColor, 1.0);
    color = theColor;
}

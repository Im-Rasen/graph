#version 330 core
//"in vec4 vertexColor;"
//uniform vec4 ourColor;
//in vec3 ourColor;
//in vec4 ourPosition;


struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
  
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

struct DirectLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectLight dirLight;

struct ProjectLight{
    vec3  position;
    vec3  direction;
    float cutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
};

uniform ProjectLight projLight;


out vec4 color;

in vec3 Normal;
in vec3 worldPosition; // Позиция текущего фрагмента
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;


//uniform sampler2D diffuseTexture;
//uniform sampler2D specularTexture;

//uniform float mixRate;



void main()
{
    //"color = vertexColor;\n"
    //color = ourColor;
    //color = ourPosition;
    //color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
    //color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), mixRate);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    vec3 norm = normalize(Normal);
    
    //Лампа
    float distance = length(light.position - worldPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));
    //Ambient
    vec3 ambient = light.ambient * vec3(texture(material.ambient, TexCoord));
    //Diffuse
    vec3 lightDirection = normalize(lightPosition - worldPosition); // Вектор от объекта к источнику
    float diffuseRate = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseRate * vec3(texture(material.diffuse, TexCoord));
    //Specular
    vec3 viewDirection = normalize(viewPosition - worldPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm); // Первый аргумент - от источника к объекту
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    //Направленный свет
    //Ambient
    vec3 dirAmbient = dirLight.ambient * vec3(texture(material.ambient, TexCoord));
    //Diffuse
    vec3 dirLightDirection = normalize(-dirLight.direction); //В итоге - на источник от объекта
    float dirDiffuseRate = max(dot(norm, dirLightDirection), 0.0);
    vec3 dirDiffuse = dirLight.diffuse * dirDiffuseRate * vec3(texture(material.diffuse, TexCoord));
    //Specular
    vec3 dirReflectDirection = reflect(-dirLightDirection, norm);
    float dirSpec = pow(max(dot(viewDirection, dirReflectDirection), 0.0), material.shininess);
    vec3 dirSpecular = dirLight.specular * dirSpec * vec3(texture(material.specular, TexCoord));
    
    // Прожектор
    vec3 projDiffuse = vec3(0.0f);
    vec3 projSpecular = vec3(0.0f);
    vec3 projAmbient = projLight.ambient * vec3(texture(material.diffuse, TexCoord));
    
    vec3 projLightDirection = normalize(projLight.position - worldPosition);
    
    float theta = dot(projLightDirection, normalize(-projLight.direction));
    if(theta > projLight.cutOff) // cos
    {
        //Diffuse
        float projDiffuseRate = max(dot(norm, projLightDirection), 0.0);
        projDiffuse = projLight.diffuse * projDiffuseRate * vec3(texture(material.diffuse, TexCoord));
        //Specular
        vec3 projReflectDirection = reflect(-projLightDirection, norm);
        float projSpec = pow(max(dot(viewDirection, projReflectDirection), 0.0), material.shininess);
        projSpecular = projLight.specular * projSpec * vec3(texture(material.specular, TexCoord));
    }
    //Attenuation
    float projDistance = length(projLight.position - worldPosition);
    float projAttenuation = 1.0 / (projLight.constant + projLight.linear * projDistance +
    projLight.quadratic * (projDistance * projDistance));
    
    

    //Финал
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    projDiffuse  *= projAttenuation;
    projSpecular *= projAttenuation;
    vec3 trueColor = projAmbient + projDiffuse + projSpecular + dirAmbient + dirDiffuse + dirSpecular;
    //+ ambient + diffuse + specular
    vec4 theColor = vec4(trueColor, 1.0);
    color = theColor;
}

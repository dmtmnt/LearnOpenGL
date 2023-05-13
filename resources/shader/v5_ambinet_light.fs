#version 330 core
out vec4 FragColor;

in vec3 FragPos; 
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{      
    //ambient 
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient) * objectColor;
    FragColor = vec4(result, 1.0);

}


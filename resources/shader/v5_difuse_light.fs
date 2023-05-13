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
   // ambient 
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   // diffuse
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);  
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

    //vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = (ambient + diffuse) * objectColor; 
    FragColor = vec4(result, 1.0);

  // FragColor = vec4(objectColor, 1.0); 
  // FragColor = vec4(lightColor * objectColor, 1.0);
}


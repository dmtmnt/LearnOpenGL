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

   // FragColor = vec4(objectColor, 1.0); 
   FragColor = vec4(lightColor * objectColor, 1.0);
}


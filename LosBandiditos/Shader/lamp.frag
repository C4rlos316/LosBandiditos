#version 330 core
out vec4 FragColor;
  
uniform vec3 lampColor; //recibir el color

void main()
{
     FragColor = vec4(lampColor, 1.0f);// Usa el color recibido
}
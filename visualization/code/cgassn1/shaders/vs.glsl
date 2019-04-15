#version 330
 
 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 FragPos; 
out vec2 TexCoord;
out vec3 Normal;
 
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 ani;

void main()
{
    Normal = normalize(vec3(viewMatrix * modelMatrix * ani * vec4(normal,0.0)));
    TexCoord = vec2(texCoord);
    gl_Position = projMatrix * viewMatrix * modelMatrix * ani * vec4(position,1.0);
	FragPos = vec3(viewMatrix*modelMatrix * ani * vec4(position,1.0));
}
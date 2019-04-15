#version 330
 
 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 ani;


layout (std140) uniform Material {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    int texCount;
};


 
uniform sampler2D texUnit;
 

uniform vec3 viewPos;

uniform vec3 light_position;
uniform vec3 light_position2;

out vec4 FragColor;


void main()
{
	vec3 FragPos; 
	vec2 TexCoord;
	vec3 Normal;

    Normal = normalize(vec3(viewMatrix * modelMatrix * ani * vec4(normal,0.0)));
    TexCoord = vec2(texCoord);
    gl_Position = projMatrix * viewMatrix * modelMatrix * ani * vec4(position,1.0);
	FragPos = vec3(viewMatrix*modelMatrix * ani * vec4(position,1.0));

	
	// light coefficient
	float light_ambient = 0.2f;
	float light_diffuse = 0.5f;
	float light_specular = 1.0f;
	float light_constant = 1.0f;
	float light_linear = 0.007f;
	float light_quadratic = 0.0002f;


    vec4 color;
    vec4 amb;
	vec4 spec;
	vec4 diff;

    float intensity;
    vec3 lightDir;
    vec3 n;

	float distance;
	float attenuation;

	// diffuse
    lightDir = normalize(light_position - FragPos);
    n = normalize(Normal);
    intensity = max(dot(lightDir,n),0.0);

    if (texCount == 0) {
        color = diffuse;
        amb = ambient;
		spec = specular;
    }
    else {
        color = texture2D(texUnit, TexCoord);
        amb = color * light_ambient;
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, n);  
		float specc = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		spec = light_specular*specc * color;  
    }


	distance = length(light_position - FragPos);
	attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));
	
	
	diff = light_diffuse * color * intensity;
 

	FragColor = (attenuation)*(amb + diff +spec);

	// diffuse
    lightDir = normalize(light_position2 - FragPos);
    n = normalize(Normal);
    intensity = max(dot(lightDir,n),0.0);

    if (texCount == 0) {
        color = diffuse;
        amb = ambient;
		spec = specular;
		FragColor = attenuation*(vec4(1.0,1.0,1.0,1.0));
    }
    else {
        color = texture2D(texUnit, TexCoord);
        amb = color * light_ambient;
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, n);  
		float specc = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		spec = light_specular*specc * color;  


		distance = length(light_position2 - FragPos);
		attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));
	
	
		diff = light_diffuse * color * intensity;
 

		FragColor += (attenuation)*(amb + diff +spec);
    }

}
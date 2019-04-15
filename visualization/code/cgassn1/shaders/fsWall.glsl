#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

in vec3 normal;

in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTextureSampler;
uniform sampler2D NormalTextureSampler;

uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightPosition_worldspace2;

void main(){

	float light_constant = 1.0f;
	float light_linear = 0.007f;
	float light_quadratic = 0.0002f;
	float attenuation;

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.5;

	// Material properties
	vec3 MaterialDiffuseColor = texture( DiffuseTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
	

	// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
	vec3 TextureNormal_tangentspace =  normalize(texture( NormalTextureSampler, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0) ;
	
	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );
	attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));

	// Normal of the computed fragment, in camera space
	//vec3 n = TextureNormal_tangentspace;
	vec3 n = normalize(normal);
	// Direction of the light (from the fragment to the light)
	//vec3 l = normalize(LightDirection_tangentspace);
	vec3 l = normalize(vec3(1.0,0.0,0.0));
	float cosTheta = clamp( abs(dot( n,l )), 0,1 );
	
	color = (
		// Ambient : simulates indirect lighting
		MaterialAmbientColor+
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta);
	//	MaterialDiffuseColor * cosTheta);
	//color = MaterialDiffuseColor * cosTheta / (distance*distance);
	//color = MaterialDiffuseColor;
	
	l = normalize(vec3(0.0,1.0,0.0));
	cosTheta = clamp( abs(dot( n,l )), 0,1 );
	
	color += (
		// Ambient : simulates indirect lighting
		MaterialAmbientColor+
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta);

		
	l = normalize(vec3(0.0,0.0,1.0));
	cosTheta = clamp( abs(dot( n,l )), 0,1 );
	
	color += (
		// Ambient : simulates indirect lighting
		MaterialAmbientColor+
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta);

	distance = length( LightPosition_worldspace2 - Position_worldspace );
	
	//attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));
	//color += attenuation*(
		// Ambient : simulates indirect lighting
	//	MaterialAmbientColor +
		// Diffuse : "color" of the object
//		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance));
		//color = MaterialDiffuseColor;

}
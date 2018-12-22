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

// Light Parameters
// constant that are used to adjust lighting
const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

// scale for restored amount of lighting
uniform float u_scaleFactor;

// coefficients of spherical harmonics and possible values

// Jake's
const float u_L00 = 0.6184374  ; 
const float u_L1m1 = -0.20878437 ; 
const float u_L10 =  0.38225156  ; 
const float u_L11 = -0.27441856 ; 
const float u_L2m2 = -0.2479079  ;
const float u_L2m1 =  0.18312824; 
const float u_L20 = 0.03431226 ; 
const float u_L21 =  0.19704065 ; 
const float u_L22 =  0.06807307; 

/*
//wonjong's
const float u_L00 = 0.11128913; 
const float u_L1m1 = 0.05943864; 
const float u_L10 = 0.31428095; 
const float u_L11 = 0.39361838; 
const float u_L2m2 = -0.29167652;
const float u_L2m1 = -0.05314109; 
const float u_L20 =  -0.27880992; 
const float u_L21 =-0.07926752; 
const float u_L22 = -0.221371523; 
*/
/*
// dahun's
const float u_L00 =  0.61462683 ; 
const float u_L1m1 = -0.27487119 ; 
const float u_L10 =  0.39332895 ; 
const float u_L11 = -0.11050449 ; 
const float u_L2m2 =  0.15947791  ;
const float u_L2m1 = 0.01785297; 
const float u_L20 =  -0.02848065 ; 
const float u_L21 = 0.33086671 ; 
const float u_L22 =  0.06498447 ; 
*/
float sphericalHarmonics(vec3 N)
{
   return
      // band 0, constant value, details of lowest frequency
      C4 * u_L00 +

      // band 1, oriented along main axes
      2.0 * C2 * u_L11 * N.x +
      2.0 * C2 * u_L1m1 * N.y +
      2.0 * C2 * u_L10 * N.z +

      // band 2, values depend on multiple axes, higher frequency details
      C1 * u_L22 * (N.x * N.x - N.y * N.y) +
      C3 * u_L20 * N.z * N.z - C5 * u_L20 +
      2.0 * C1 * u_L2m2 * N.x * N.y +
      2.0 * C1 * u_L21 * N.x * N.z +
      2.0 * C1 * u_L2m1 * N.y * N.z;
}

void main(){

	float light_constant = 1.0f;
	float light_linear = 0.007f;
	float light_quadratic = 0.0002f;
	float attenuation;

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 0.3;

	// Material properties
	vec3 MaterialDiffuseColor = texture( DiffuseTextureSampler, UV ).rgb;
	
	MaterialDiffuseColor = vec3(0.5,0.5,0.5);
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
	

	// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
    vec3 TextureNormal_tangentspace =  normalize(texture( NormalTextureSampler, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0);

	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );
	attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));

	// Normal of the computed fragment, in camera space
	vec3 n = TextureNormal_tangentspace;
	//vec3 n = normalize(normal);
	// Direction of the light (from the fragment to the light)
	//vec3 l = normalize(LightDirection_tangentspace);

	color = (
		MaterialAmbientColor+
		MaterialDiffuseColor * LightColor * LightPower * sphericalHarmonics(n));
		
	vec3 l = normalize(vec3(1.0,0.0,0.0));
	float cosTheta = clamp( abs(dot( n,l )), 0,1 );
	
	color += (
		MaterialAmbientColor+
		MaterialDiffuseColor * LightColor * LightPower * cosTheta);


	
	 l = normalize(vec3(0.0,1.0,0.0));
	 cosTheta = clamp( abs(dot( n,l )), 0,1 );
	
	color += (
		MaterialAmbientColor+
		MaterialDiffuseColor * LightColor * LightPower * cosTheta);
	
	

		
}
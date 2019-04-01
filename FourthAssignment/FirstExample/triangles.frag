#version 430 core
in vec3 myColor;
in vec2 texCoord;

in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec4 fColor;

uniform sampler2D texture0;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform highp mat4 V; 

void main()
{
	vec3 totalLight = vec3(0,0,0);
	vec3 n = normalize(Normal_cameraspace);
	vec3 LightPosition_cameraspace = ( V * vec4(lightPosition, 1.0)).xyz;
	vec3 lightDirection = LightPosition_cameraspace + EyeDirection_cameraspace;

	vec3 l = normalize(lightDirection);
	float diff = max(dot(n,l), 0.0);
	vec3 diffuse =  diff * lightColor;
	totalLight += diffuse;

	vec3 E = normalize(EyeDirection_cameraspace);
	vec3 R = reflect(-l,n);

	float cosAlpha = clamp(dot(E,R), 0,1);
	vec3 specular = lightColor * 1000 * pow(cosAlpha, 32);
	totalLight += specular;

	vec3 ambient = vec3(0.2,0.2,0.2);

	fColor = vec4((ambient + totalLight) * texture(texture0, texCoord).rgb,1.0);
}
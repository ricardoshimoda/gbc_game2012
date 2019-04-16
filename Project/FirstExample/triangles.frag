
#version 430 core
in vec2 texCoord; // Same thing as UV
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;

out vec4 fColor;

// Define the number of points of light
#ifndef NUM_POINT_LIGHTS
    #define NUM_POINT_LIGHTS 3
#endif

struct Light{
    vec3 Color;
    vec3 Position;    
    float Strength;
    float falloffStart;
    float falloffEnd;
};

uniform sampler2D texture0; // same thing
// Replaced by the struct 
//uniform vec3 lightColor;
//uniform vec3 lightPosition;

uniform Light pointLights[NUM_POINT_LIGHTS];
uniform highp mat4 V; 

vec3 calcPointLights(){
    vec3 totalLight = vec3(0);
    for (int i =0; i<NUM_POINT_LIGHTS;i++){
        /*******************
         *Calculate diffuse
         *******************/
        // Distance to the light
        float distance = length( pointLights[i].Position - Position_worldspace );
        // Range test.
        if(distance <= pointLights[i].falloffEnd){
            // Normal of the computed fragment, in camera space
            vec3 n = normalize( Normal_cameraspace );
            
            // Direction of the light (from the fragment to the light)
            vec3 LightPosition_cameraspace = ( V * vec4(pointLights[i].Position,1)).xyz;
            vec3 lightDirection = LightPosition_cameraspace + EyeDirection_cameraspace;
            
            vec3 l = normalize( lightDirection );
            // Cosine of the angle between the normal and the light direction,
            // clamped above 0
            //  - light is at the vertical of the triangle -> 1
            //  - light is perpendicular to the triangle -> 0
            //  - light is behind the triangle -> 0
            //float cosTheta = clamp( dot( n,l ), 0,1 );
            
            float diff = max(dot(n, l), 0.0);
            vec3 diffuse = pointLights[i].Color * pointLights[i].Strength * diff * ((pointLights[i].falloffEnd-distance) / (pointLights[i].falloffEnd - pointLights[i].falloffStart));
            totalLight += diffuse;
            
            /*******************
             *Calculate specular
             *******************/
            // Specular Strength
            float specStrength = 10;
            // Eye vector (towards the camera)
            vec3 E = normalize(EyeDirection_cameraspace);
            // Direction in which the triangle reflects the light
            vec3 R = reflect(-l,n);
            // Cosine of the angle between the Eye vector and the Reflect vector,
            // clamped to 0
            //  - Looking into the reflection -> 1
            //  - Looking elsewhere -> < 1
            float cosAlpha = max( dot( E,R ), 0);
            vec3 specular = pointLights[i].Color * specStrength * pow(cosAlpha,32) / (distance*distance);
            totalLight += specular;
        }
    }

    return totalLight;
}
void main()
{
    vec3 ambient = vec3(0.2,0.2,0.2);
	//fColor = vec4(texture(texture0, texCoord).rgb,1.0);
    fColor = vec4((ambient+calcPointLights()) * texture(texture0, texCoord).rgb,1.0);

}
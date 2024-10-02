#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float heightScale;
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float height = texture(depthMap, texCoords).r;
    return texCoords - viewDir.xy * (height * heightScale);
}

// vec2 ParallaxMappingSteep(vec2 texCoords, vec3 viewDir)
// {
//     const float minLayers = 8.0;
//     const float maxLayers = 32.0;
//     float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));  
//     float layerDepth = 1.0/numLayers;
//     float currentLayerDepth = 0.0;
//     vec2 P = viewDir.xy 

// }
void main()
{           

     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.9 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

// void main()//ParallaxMapping
// {
//     vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
//     vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
    // if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    //     discard;

//     vec3 normal = texture(normalMap, texCoords).rgb;
//     normal = normalize(normal * 2.0 - 1.0);

//     vec3 color = texture(diffuseMap, texCoords).rgb;

//     vec3 ambient = 0.3 * color;

//     vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

//     float diff = max(dot(lightDir, normal), 0.0);

//     vec3 diffuse = diff * color;

//     vec3 halfwayDir = normalize(lightDir + viewDir);
//     float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);

//     vec3 specular = vec3(0.2) * spec;

//     FragColor = vec4(ambient + diffuse + specular, 1.0);
// }
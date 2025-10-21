#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_NormalMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
} vs_out;

void main() {
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    vs_out.FragPos = worldPos.xyz;
    vs_out.TexCoords = a_TexCoords;
    
    // Calculate TBN matrix for normal mapping
    vec3 T = normalize(vec3(u_Transform * vec4(a_Tangent, 0.0)));
    vec3 B = normalize(vec3(u_Transform * vec4(a_Bitangent, 0.0)));
    vec3 N = normalize(vec3(u_Transform * vec4(a_Normal, 0.0)));
    vs_out.TBN = mat3(T, B, N);
    
    vs_out.Normal = N;
    vs_out.Tangent = T;
    vs_out.Bitangent = B;
    
    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
} fs_in;

// Material properties (Ray Tracing focused)
struct Material {
    vec3 Albedo;
    float Metallic;
    float Roughness;
    float AO;
    vec3 Emission;
    float EmissionStrength;
    float IOR;
    float Transmission;
};

uniform Material u_Material;

// Texture maps
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicRoughnessMap;
uniform sampler2D u_AOMap;
uniform sampler2D u_EmissionMap;

uniform int u_UseAlbedoMap;
uniform int u_UseNormalMap;
uniform int u_UseMetallicRoughnessMap;
uniform int u_UseAOMap;
uniform int u_UseEmissionMap;

// Lighting
uniform vec3 u_CameraPos;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;

const float PI = 3.14159265359;

// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

// Geometry function (Smith's method)
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

void main() {
    // Sample textures
    vec3 albedo = u_Material.Albedo;
    if (u_UseAlbedoMap == 1) {
        albedo = texture(u_AlbedoMap, fs_in.TexCoords).rgb;
        albedo = pow(albedo, vec3(2.2)); // Gamma correction
    }
    
    vec3 normal = normalize(fs_in.Normal);
    if (u_UseNormalMap == 1) {
        normal = texture(u_NormalMap, fs_in.TexCoords).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(fs_in.TBN * normal);
    }
    
    float metallic = u_Material.Metallic;
    float roughness = u_Material.Roughness;
    if (u_UseMetallicRoughnessMap == 1) {
        vec3 mr = texture(u_MetallicRoughnessMap, fs_in.TexCoords).rgb;
        metallic = mr.b;
        roughness = mr.g;
    }
    
    float ao = u_Material.AO;
    if (u_UseAOMap == 1) {
        ao = texture(u_AOMap, fs_in.TexCoords).r;
    }
    
    vec3 emission = u_Material.Emission * u_Material.EmissionStrength;
    if (u_UseEmissionMap == 1) {
        emission = texture(u_EmissionMap, fs_in.TexCoords).rgb * u_Material.EmissionStrength;
    }
    
    // Calculate reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    // View direction
    vec3 V = normalize(u_CameraPos - fs_in.FragPos);
    vec3 L = normalize(-u_LightDirection);
    vec3 H = normalize(V + L);
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, H, roughness);
    float G = GeometrySmith(normal, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    float NdotL = max(dot(normal, L), 0.0);
    vec3 Lo = (kD * albedo / PI + specular) * u_LightColor * u_LightIntensity * NdotL;
    
    // Ambient lighting
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo + emission;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
}

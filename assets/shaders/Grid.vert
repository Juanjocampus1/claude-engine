#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

out vec3 v_NearPoint;
out vec3 v_FarPoint;

// Grid position are in clipped space
vec3 UnprojectPoint(float x, float y, float z, mat4 viewProjInv) {
    vec4 unprojectedPoint = viewProjInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    mat4 viewProjInv = inverse(u_ViewProjection);
    
    // Generate grid plane in clip space
    v_NearPoint = UnprojectPoint(a_Position.x, a_Position.y, 0.0, viewProjInv).xyz; // Near plane
    v_FarPoint = UnprojectPoint(a_Position.x, a_Position.y, 1.0, viewProjInv).xyz;  // Far plane
    
    gl_Position = vec4(a_Position.xy, 0.0, 1.0); // Full screen quad
}

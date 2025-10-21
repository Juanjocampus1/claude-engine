#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

out vec3 v_NearPoint;
out vec3 v_FarPoint;

vec3 UnprojectPoint(float x, float y, float z, mat4 viewProj) {
    mat4 viewProjInv = inverse(viewProj);
    vec4 unprojectedPoint = viewProjInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = a_Position;
    v_NearPoint = UnprojectPoint(p.x, p.y, 0.0, u_ViewProjection).xyz;
    v_FarPoint = UnprojectPoint(p.x, p.y, 1.0, u_ViewProjection).xyz;
    gl_Position = vec4(p, 1.0);
}

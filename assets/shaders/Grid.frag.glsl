#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_NearPoint;
in vec3 v_FarPoint;

uniform mat4 u_ViewProjection;
uniform float u_Near;
uniform float u_Far;

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    
    // X axis (red)
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color = vec4(0.8, 0.0, 0.0, 1.0);
    
    // Z axis (blue)
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color = vec4(0.0, 0.3, 0.8, 1.0);
        
    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = u_ViewProjection * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos) {
    float near = u_Near;
    float far = u_Far;
    vec4 clip_space_pos = u_ViewProjection * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    return linearDepth / far;
}

void main() {
    float t = -v_NearPoint.y / (v_FarPoint.y - v_NearPoint.y);
    vec3 fragPos3D = v_NearPoint + t * (v_FarPoint - v_NearPoint);
    
    gl_FragDepth = computeDepth(fragPos3D);
    
    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));
    
    FragColor = grid(fragPos3D, 1.0) * float(t > 0);
    FragColor.a *= fading;
}

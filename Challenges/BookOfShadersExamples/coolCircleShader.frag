// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

vec3 normalizedColor(vec2 st, float radius, float falloff, vec3 falloffColor) {
    float falloffThreshold = radius - falloff;
    if (length(st) < falloffThreshold) {
        return vec3(1.0);
    }
    vec3 normalColor = vec3(1.0);
    
    float innerColorPct = (radius - length(st)) / falloff;
    vec3 edgeColor = (innerColorPct * falloffColor);
    
    return smoothstep(normalColor, edgeColor, vec3(falloffThreshold));
}

void main() 
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    st = (st * vec2(2.0)) - vec2(1.0);

    float theta = u_time;
    float innerRadius = 0.104;// * (abs(sin(u_time / 1.0)) * 0.2 + 0.8);
    float innerFalloff = 0.028;
    vec3 innerFalloffColor = vec3(0.450,0.763,0.810);
    vec3 innerColor = normalizedColor(st, innerRadius, innerFalloff, innerFalloffColor);
    
    float outerRadius = 0.580;// * (abs(cos(u_time / 10.0)) * 0.2 + 0.8);;
    float outerFalloff = 0.028;
    vec3 outerFalloffColor = vec3(0.434,0.550,0.427);
    vec3 outerColor = normalizedColor(st, outerRadius, outerFalloff, outerFalloffColor);
	
    float pos = length(st);
    float interpolation = 0.064 - ((innerRadius - (outerRadius - pos)) / (innerRadius - outerRadius));
    gl_FragColor = vec4(mix(innerColor, outerColor, interpolation), 1.0);
}
#version 460 core
out vec4 FragColor;

layout (location = 0) in vec2 fragCoord;

#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

void main() {
    vec2 st = fragCoord.xy/u_resolution;
    vec3 color = vec3(0.0);

    // Use polar coordinates instead of cartesian
    float maxRadius = 0.600;
    vec2 toCenter = vec2(0.5)-st;
    float angle = atan(toCenter.y,toCenter.x);
    float radius = (length(toCenter)*2.0 / maxRadius);

    // Map the angle (-PI to PI) to the Hue (from 0 to 1)
    // and the Saturation to the radius
    float finalAngle = (angle/TWO_PI)+(u_time/3.0);
    color = hsb2rgb(vec3(finalAngle,radius,1.0));
	
    vec2 normalSt = (st * vec2(2.0)) - vec2(1.0);
    vec3 colorMultiplier = vec3(length(normalSt) < maxRadius);
    vec4 colorWheel = vec4(color*colorMultiplier,1.0);
    
    float borderWidth = 0.020;
    bool isBorder = length(normalSt) < maxRadius + borderWidth && length(normalSt) >= maxRadius;
    vec4 borderColor = isBorder ? vec4(0.489,0.464,0.505,1.000) : vec4(0.0);
    FragColor = borderColor + colorWheel;
}

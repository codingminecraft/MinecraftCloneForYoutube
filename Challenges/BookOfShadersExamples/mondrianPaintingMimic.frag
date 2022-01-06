// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

float rectangle(vec2 size, vec2 position, float blurRadius) {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    
    // bottom-left
    vec2 edge0 = position - (size/2.0) - blurRadius;
    vec2 edge1 = position - (size/2.0) + blurRadius;
    vec2 bl = smoothstep(edge0, edge1,st);
    float pct = bl.x * bl.y;

    // top-right
    vec2 edge2 = position + (size/2.0) - blurRadius;
    vec2 edge3 = position + (size/2.0) + blurRadius;
    vec2 tr = vec2(1.0) - smoothstep(edge2, edge3,st);
    pct *= tr.x * tr.y;
    
    return pct;
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);
	
    const int numRects = 6;
    vec2 sizes[numRects];
    sizes[0] = vec2(0.630,0.620);
    sizes[1] = vec2(0.510,0.330);
    sizes[2] = vec2(0.300,0.470);
    sizes[3] = vec2(0.400,0.680);
    sizes[4] = vec2(0.450,0.760);
    sizes[5] = vec2(0.310,0.500);
    vec2 positions[numRects];
    positions[0] = vec2(0.040,0.890);
    positions[1] = vec2(0.670,0.900);
    positions[2] = vec2(0.570,0.450);
    positions[3] = vec2(0.990,0.350);
    positions[4] = vec2(0.140,0.150);
    positions[5] = vec2(0.590,-0.080);
    float blurRadius = 0.002;
    vec3 colors[numRects];
    colors[0] = vec3(0.151,0.350,1.000);
    colors[1] = vec3(1.000,0.970,0.851);
    colors[2] = vec3(1.000,0.970,0.851);
    colors[3] = vec3(1.000,0.269,0.250);
    colors[4] = vec3(1.000,0.970,0.851);
    colors[5] = vec3(1.000,0.925,0.292);
    float rectangles[numRects];

    for (int i = 0; i < numRects; i++) {
        float rValue = rectangle(sizes[i], positions[i], blurRadius);
        if (rValue > 0.0) {
            color = colors[i];
        }
    }

    gl_FragColor = vec4(color,1.0);
}

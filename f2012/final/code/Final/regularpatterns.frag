/*
 * Regular procedural patterns 
 * 
*/

uniform float blotchyness;
uniform float Option;

varying vec4 texCoord;

void main() 
{
    float s = texCoord[0];
    float t = texCoord[1];
    vec2 st = texCoord.st;
    
    float pattern; 
    
    if (Option == 0.0) {        
        // gradient
        pattern  = s;
        
    } else if (Option == 1.0) {
        // lines
        pattern = fract(10.0*s);

    } else if (Option == 2.0) {    
        // ridges
        pattern = abs(fract(5.0*s) * 2.0 - 1.0);

    } else if (Option == 3.0) {        
        // checker
        pattern = mod(floor(10.0*s) + floor(10.0*t), 2.0);

    } else if (Option == 4.0) {    
        // sine wave on bottom
        pattern = smoothstep(-0.01, 0.01, 0.2 - 0.1*sin(30.0*s) - t);

    } else if (Option == 5.0) {
        // dots
        pattern = smoothstep(0.3, 0.32, length(fract(5.0*st) - 0.5));
        
    } else {    
        // bricks
        pattern = smoothstep(0.4, 0.5, max(abs(fract(8.0*s - 0.5*mod(floor(8.0*t), 2.0)) - 0.5), abs(fract(8.0*t) - 0.5)));
        
    }
    gl_FragColor = vec4(vec3(pattern), 1.0);
    
}
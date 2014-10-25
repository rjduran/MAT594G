
uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;

varying vec2 MCposition;
varying float LightIntensity;


void main()
{
    
    vec3 color;
    vec2 position, useBrick;
    
    // divide the fragments x position in modeling coords by the brick column width
    //              and the y position in modeling coords by the brick row height 
    // position.y is the brick row number
    // position.x is the brick number in that row
    position = MCposition / BrickSize;
    
    if (fract(position.y * 0.5) > 0.5) {
        position.x += 0.5;
    }
    
    position = fract(position);
    
    // in the brick or mortar?
    // 0 when BrickPct.x < position.x  : 0.90 <
    // 1 when BrickPct.x >= position.x : 0.90 >=
    
    
    useBrick = step(position, BrickPct);
    
    color = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    color *= LightIntensity;
    
    //    vec3 color2 = vec3(0.0, 0.8, 0.95);
    //    color2 *= LightIntensity;
    //    gl_FragColor = vec4(color2, 1.0); 
    
    gl_FragColor = vec4(color, 1.0); 
}
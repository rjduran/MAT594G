
///////////////////////////////////////////////////////////////////////
// checker pattern
///////////////////////////////////////////////////////////////////////    

varying vec4 texCoord;

void main() 
{
    float s = texCoord[0];
    float t = texCoord[1];
    
    float fval = 0.0; // output value
    
    float freq = 2.0;
    float smod = mod(freq*s, 1.0);
    float tmod = mod(freq*t, 1.0);
    if (smod < 0.5) {
        if (tmod < 0.5) {
            fval = 1.0;
        } else {
            fval = 0.0;
        } 
    } else {
        if (tmod < 0.5) {
            fval = 0.0;
        } else {
            fval = 1.0;
        } 
    }
    
    gl_FragColor = vec4(vec3(fval), 1.0);
    
}
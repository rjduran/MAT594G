//varying vec4 texCoord;
//
//varying vec3 Position;
//
//uniform float brightness;
//uniform bool swapColor;

void main()
{
    

    
//    float real = Position.x * Zoom + Xcenter;
//    float imag = Position.y * Zoom + Ycenter;
//    
//    
//    float Creal = creal; // store as uniform to allow it to be changed
//    float Cimag = cimag;
//    
//    float r2 = 0.0;
//    float iter;
//    
//    for(iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
//    {
//        float tempreal = real;
//        
//        real = (tempreal * tempreal) - (imag * imag) + Creal;
//        imag = 2.0 * tempreal * imag + Cimag;
//        
//        r2 = (real * real) + (imag * imag);
//    }
//    
//    vec3 color;
//    
//    if(r2 < 4.0) {
//        color = InnerColor;
//    } else {
//        color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));
//    }
    
    
    
    ///////////////////////////////////////////////////////////////////////
    // checker pattern
    ///////////////////////////////////////////////////////////////////////    
//    float s = texCoord[0];
//    float t = texCoord[1];
//    
//    float fval = 0.0; // output value
//    
//    //float freq = 2.0;
//    float smod = mod(freq*s, 1.0);
//    float tmod = mod(freq*t, 1.0);
//    if (smod < 0.5) {
//        if (tmod < 0.5) {
//            fval = 1.0;
//        } else {
//            fval = 0.0;
//        } 
//    } else {
//        if (tmod < 0.5) {
//            fval = 0.0;
//        } else {
//            fval = 1.0;
//        } 
//    }
//    
//    gl_FragColor = vec4(vec3(fval), 1.0);
    
    
    ///////////////////////////////////////////////////////////////////////
    // polka dot pattern
    ///////////////////////////////////////////////////////////////////////
//    float s = texCoord[0];
//    float t = texCoord[1];
//    
//    float fval = 0.0; // output value
//    
//    //float freq = 10.0;
//    float r = 0.5;
//    float smod = mod(freq*s, 1.0);
//    float tmod = mod(freq*t, 1.0);
//    if ((smod - r)*(smod - r) + (tmod - r)*(tmod - r) <= r*r) {
//        fval = 1.0;
//    } else {    
//        fval = 0.0;
//    }
//    
//    gl_FragColor = vec4(vec3(fval), 1.0);
    
    
    ///////////////////////////////////////////////////////////////////////
    // sine wave pattern
    ///////////////////////////////////////////////////////////////////////
//    float s = texCoord[0];
//    float t = texCoord[1];
//    
//    s = (sin(2.0 * 3.14159265 * freq * s) + 1.0) * 0.5;
    
    /*
    t = (sin(freq * t * 3.14159265) + 1.0) * 0.5;
    
    float fval = 0.0; // output value
    
    float freq2 = 10.0;
    float smod = mod(freq2*s, 1.0);
    float tmod = mod(freq2*t, 1.0);
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
    */
    
//    gl_FragColor = vec4(vec3(s), 1.0);

    
    ///////////////////////////////////////////////////////////////////////
    // swap colors
    ///////////////////////////////////////////////////////////////////////     
//    if(swapColor) {
//        color = vec4(vec3(brightness, 0.0, 0.0), 1.0);
//    } else {
//        color = vec4(vec3(1.0, 0.0, brightness), 1.0);
//    }
//    
//    gl_FragColor = color;
        
    
    ///////////////////////////////////////////////////////////////////////
    // colors
    ///////////////////////////////////////////////////////////////////////         

    // these are from s: [0, 1], t: [0, 1]
    //float s = texCoord[0];
    //float t = texCoord[1];
    
    // these are from x: [-2.5, 2.5], y: [-2.5, 2.5]
    // 0, 0 in center
//    float x = Position.x;
//    float y = Position.y;
//    
//    if (sqrt(x*x + y*y) < 1.0) {
//        gl_FragColor = vec4(s, t, 0.0, 1.0);    
//    } else {
//        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
//    }

    
    
//    float s = texCoord[0];
//    float t = texCoord[1];
//    
//    float fval = 0.0; // output value
//    
//    
//    //float freq = 2.0;
//    float smod = mod(freq*s, 1.0);
//    float tmod = mod(freq*t, 1.0);
//    if (smod < 0.5) {
//        if (tmod < 0.5) {
//            fval = 1.0; // square val            
//        } else {
//            fval = 0.0;
//        } 
//    } else {
//        if (tmod < 0.5) {
//            fval = 0.0;
//        } else {
//            fval = 1.0;
//        } 
//    }
//    
//    
//    float freq2 = 1.0;
//    float r = 0.25;
//    smod = mod(freq2*s, 1.0)-0.5;
//    tmod = mod(freq2*t, 1.0)-0.5;
//    if ((smod - r)*(smod - r) + (tmod - r)*(tmod - r) <= r*r) {
//        fval = fval-1.0;
//    } else {    
//        fval = fval;
//    }
//    
//
//    gl_FragColor = vec4(vec3(fval), 1.0);

    
}















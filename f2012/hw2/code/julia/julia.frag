varying vec4 texCoord;

varying vec3 Position;

uniform float MaxIterations;
uniform float Zoom;
uniform float Xcenter;
uniform float Ycenter;
uniform vec3 InnerColor;
uniform vec3 OuterColor1;
uniform vec3 OuterColor2;

uniform float creal;
uniform float cimag;


void main()
{
  
  float real = Position.x * Zoom + Xcenter;
  float imag = Position.y * Zoom + Ycenter;

  
  float Creal = creal; // store as uniform to allow it to be changed
  float Cimag = cimag;

  float r2 = 0.0;
  float iter;

  for(iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
  {
    float tempreal = real;
    
    real = (tempreal * tempreal) - (imag * imag) + Creal;
    imag = 2.0 * tempreal * imag + Cimag;
    
    r2 = (real * real) + (imag * imag);
  }

  vec3 color;

  if(r2 < 4.0) {
    color = InnerColor;
  } else {
    color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));
  }

  gl_FragColor = vec4(color, 1.0);
  
}















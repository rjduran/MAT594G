varying vec4 texCoord;

varying vec3 Position;

void main()
{
  Position = vec3(gl_MultiTexCoord0 - 0.5) * 5.0;  
  gl_Position = ftransform();

  texCoord = gl_MultiTexCoord0;
}



varying vec4 texCoord;

void main()
{        
    texCoord = gl_MultiTexCoord0;
    gl_Position = ftransform();
}
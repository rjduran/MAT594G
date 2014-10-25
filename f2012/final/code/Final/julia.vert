//varying vec4 texCoord;
//varying vec3 Position;




/*
void main()
{
    // -2.5 to 2.5 in s and t directions with (0.0, 0.0) at center
    // Position = vec3(gl_MultiTexCoord0 - 0.5) * 5.0; 
  
    Position = vec3(gl_MultiTexCoord0) * 10.0; 
    
    texCoord = gl_MultiTexCoord0;
    
    
    xpos = (gl_Vertex.x + 1.0)*0.5;
    //xpos = gl_Vertex.x;
    //ypos = clamp(gl_Vertex.y,0.0,1.0);
    
    // these are all equivalent
    //gl_Position = ftransform();
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
*/
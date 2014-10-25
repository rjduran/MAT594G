void main()
{
  // pass through the texture coordinate
  gl_TexCoord[0] = gl_MultiTexCoord0;
  
  // pass through the quad position
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

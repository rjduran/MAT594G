varying float depth;
varying vec3 normal;
varying vec3 eye;
varying vec3 lightPosition;

void main()
{
  // get the depth and eye position
  vec4 transformedVertex = gl_ModelViewMatrix * gl_Vertex;
  depth = -transformedVertex.z;
  eye = -transformedVertex.xyz;

  // transform normals to the current view
  normal = normalize(gl_NormalMatrix * normalize(gl_Normal));

  // pass the light position through
  lightPosition = vec3(gl_LightSource[0].position);

  gl_Position = ftransform();
}


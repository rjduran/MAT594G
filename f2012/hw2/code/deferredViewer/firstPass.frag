varying float depth;
varying vec3 normal;
varying vec3 eye;
varying vec3 lightPosition;

void main()
{
  // these values are set empirically based on the dimensions
  // of the bunny model
  float near = 1.3;
  float far = 2.5;

  // get shifted versions for better visualization
  float depthShift = (depth - near) / (far - near);
  vec3 normalShift = (normal + vec3(1.0)) * 0.5;

  gl_FragData[0] = vec4(normalShift, depthShift);
  gl_FragData[1] = vec4(normalize(eye), 1.0);
  gl_FragData[2] = vec4(normalize(lightPosition), 1.0);  
}

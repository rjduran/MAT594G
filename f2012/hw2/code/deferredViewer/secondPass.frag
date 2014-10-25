// sampler textures
uniform sampler2D depthTexture;
uniform sampler2D eyeTexture;
uniform sampler2D lightTexture;

vec4 color;

void main() 
{

  // pull everything we want from the textures for this pixel
  float depth   = texture2D(depthTexture, gl_TexCoord[0].st).a;
  vec3 normal   = texture2D(depthTexture, gl_TexCoord[0].st).rgb;  //gl_TexCoord[0].st is a vec2
  vec3 eye      = texture2D(eyeTexture,   gl_TexCoord[0].st).rgb;
  vec3 light    = texture2D(lightTexture, gl_TexCoord[0].st).rgb;


  // 1.2.1 Ink Outlines
  float d = 0.002;
  vec2 texc = gl_TexCoord[0].st; // texture coordinate (x,y) of current pixel
  
  float east = texture2D(depthTexture, texc + vec2(d, 0)).a;
  float west = texture2D(depthTexture, texc + vec2(-d, 0)).a;
  float north = texture2D(depthTexture, texc + vec2(0, d)).a;
  float south = texture2D(depthTexture, texc + vec2(0, -d)).a;
  
  // smooth artifacts  
  vec3 eastNormal = texture2D(depthTexture, texc + vec2(d, 0)).rgb;
  vec3 westNormal = texture2D(depthTexture, texc + vec2(-d, 0)).rgb;
  vec3 northNormal = texture2D(depthTexture, texc + vec2(0, d)).rgb;
  vec3 southNormal = texture2D(depthTexture, texc + vec2(0, -d)).rgb;
  vec3 normAvg = (normal + eastNormal + westNormal + northNormal + southNormal)*0.2;

  // repackage the normal component
  normal = normAvg * 2.0 - vec3(1.0);
  normal = normalize(normal);
 
  // get a diffuse lighting component
  float diffuse = dot(normal, light);

  // make it gray
  //diffuse *= 0.5;
    
  // if the difference between the current depth and it's neighbor is large, shade black
  float th = 0.01;
  
  float avg = (depth + east + west + north + south)*0.2;
  if (depth - avg > th) {
    // black
    color = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    // other color
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    
    // 1.2.2 Cartoon Lighting
    vec3 R = reflect(-light, normal);  
    float spec = pow( max(dot(R, eye), 0.0), 15.0 );

    if (diffuse <= 0.5) {
      diffuse = 0.0;
    } else {
      diffuse = 0.5;
    }

    if (spec <=0.5) {
      spec = 0.0;
    } else {
      spec = 1.0;
    }
    
    vec4 Id = vec4(vec3(diffuse), 1.0);  // diffuse
    vec4 Is = vec4(vec3(spec), 1.0);     // specular
    
    color = Id + Is;

  }
  
  gl_FragColor = color;
    
}

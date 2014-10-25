
uniform vec3 LightPosition;

const float SpecularContribution = 0.5;
const float DiffuseContribution = 1.0 - SpecularContribution;

varying float LightIntensity;
varying vec2 MCposition;


void main()
{
    // eye coordinate position
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    
    // transform the incomming normal for specular reflection
    vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);
    
    // light direction vector
    vec3 lightVec = normalize(LightPosition - ecPosition);
    
    // reflection vector
    vec3 reflectVec = reflect(-lightVec, tnorm);
    
    // vector from object to viewer
    vec3 viewVec = normalize(-ecPosition);
    
    // diffuse component of the light intensity
    float diffuse = max(dot(lightVec, tnorm), 0.0);
    
    // specular component of the light intensity
    float spec = 0.0;
    if (diffuse > 0.0) {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 10.0);
    }
    
    LightIntensity = DiffuseContribution * diffuse + SpecularContribution * spec;
    MCposition = gl_Vertex.xy;
    
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
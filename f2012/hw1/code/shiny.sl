surface shiny()
{
  float kd = 0.5; // diffuse
  float ks = 0.5; // specular  
  float ksmod;    // specular modulation

  color specularcolor = 1;
  float roughness = 1;

  point Nf = faceforward(normalize(N), I);

  float freq = 20;
  float umod = mod(freq*u, 1);
  float vmod = mod(freq*v, 1);
  if (umod < 0.5) {
    if (vmod < 0.5) {      
      ksmod = 1;
    } else {      
      ksmod = 0;
    } 
  } else {
    if (vmod < 0.5) {      
      ksmod = 0;
    } else {      
      ksmod = 1;
    } 
  }

  Oi = Os;
  Ci = Os * (kd * diffuse(Nf) + specularcolor * ks * ksmod * specular(Nf, -normalize(I), roughness) );  
}
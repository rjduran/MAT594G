surface measles()
{
  float Kd = 1; // diffuse
  float Ka = 1; // ambient
  
  float d; // distance

  point Nf = faceforward(normalize(N), I);
  
  float freq = 20;
  float umod = mod(freq*u, 1);
  float vmod = mod(freq*v, 1);
  
  if ((umod - 0.5)*(umod - 0.5) + (vmod - 0.5)*(vmod - 0.5) <= 0.25) {
    point center = (0.5, 0.5, 0);
    d = distance(center, (umod, vmod, 0));
    Ci = color(1, 0, 0) - smoothstep(0, 0.5, d);
    
  } else {  
    Ci = color(0,0,0);     
  }

  Oi = Os;
  Ci = Oi * Ci * (Kd * diffuse(Nf));
}
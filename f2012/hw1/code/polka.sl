surface polka()
{  
  float Kd = 1; // diffuse
  float Ka = 1; // ambient

  point Nf = faceforward(normalize(N), I);

  float freq = 20;
  float r = 0.5;
  float umod = mod(freq*u, 1);
  float vmod = mod(freq*v, 1);  
  
  if ((umod - r)*(umod - r) + (vmod - r)*(vmod - r) <= r*r) {
    Ci = Cs;
  } else {    
    Ci = color(0,0,0);     
  }

  Oi = Os;
  Ci = Oi * Ci * (Kd * diffuse(Nf));
}

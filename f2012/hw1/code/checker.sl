surface checker()
{
  float Kd = 1; // diffuse
  float Ka = 1; // ambient

  point Nf = faceforward(normalize(N), I);
   
  float freq = 20;
  float umod = mod(freq*u, 1);
  float vmod = mod(freq*v, 1);
  if (umod < 0.5) {
    if (vmod < 0.5) {
      Ci=Cs;
    } else {
      Ci=color(0,0,0);
    } 
  } else {
    if (vmod < 0.5) {
      Ci=color(0,0,0); 
    } else {
      Ci=Cs;
    } 
  }

  Oi = Os;
  Ci = Oi * Ci * (Ka * ambient() + Kd * diffuse(Nf));
}
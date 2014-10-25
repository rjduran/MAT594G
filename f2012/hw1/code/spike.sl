displacement spike()
{
  point Nf = faceforward(normalize(N), I);
  
  float freq = 20;
  float scale = 0.25;
  float h = 0;
  float d; // distance
  float umod = mod(freq*u, 1);
  float vmod = mod(freq*v, 1);
  
  if ((umod - 0.5)*(umod - 0.5) + (vmod - 0.5)*(vmod - 0.5) <= 0.25) {
    point center = (0.5, 0.5, 0);
    d = distance(center, (umod, vmod, 0));
    h = (1 - smoothstep(0, 0.5, d))*scale;
    
  } else {  
    h = 0;
  }

  P = P + normalize(N) * h; 
  N = calculatenormal(P);
}

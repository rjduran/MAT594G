displacement bunny()
{   
  float Kd = 1; // diffuse
  float Ka = 1; // ambient

  N = calculatenormal(P);
  point Nf = faceforward(normalize(N), I);

  float freq = 20;
  float x = xcomp(P);
  float y = ycomp(P);
  float z = zcomp(P);

  float xmod = mod(freq*x, 1);
  float ymod = mod(freq*y, 1);
  float zmod = mod(freq*z, 1);
  
  if ((xmod - 0.5)*(xmod - 0.5) + (ymod - 0.5)*(ymod - 0.5) + (zmod - 0.5)*(zmod - 0.5) <= 0.25) {    
    Ci = color(1, 0, 0);
  } else {    
    Ci = color(1, 1, 0); 
  }

  Oi = Os;
  Ci = Oi * Ci * (Ka * ambient() + Kd * diffuse(Nf));
}



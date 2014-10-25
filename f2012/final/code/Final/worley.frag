/*
 * Worley noise
 * 
 * Based on textures described in Procedural Textures in GLSL by Stefan Gustavson.
 *
 */


varying vec4 texCoord;

uniform float Option;

uniform float Amplitude;
uniform float Scale;

uniform float Shape;
uniform float Depth;
uniform vec3 ColorVar;
uniform float NRings;
uniform float Blotchyness;

float pattern;


// functions
vec3 permute(vec3 x);
vec2 cellular(vec2 P);
float aastep (float threshold , float value);
vec4 permute(vec4 x);

float snoise(vec3 v);


void main() 
{
    float s = texCoord[0];
    float t = texCoord[1];

    vec2 st = texCoord.st;    

    
    if (Option == 0.0) {                    

        // worley_cell1_ver1 : cellular 1 (from paper) with color mod
        vec2 F = cellular(st * 20.0);    
        gl_FragColor = vec4(vec3(F.y, F.x, 0.5), 0.1);
    
        
    } else if (Option == 1.0) {
        
        // worley_cell1_ver2
        vec2 F = cellular(st * 50.0);    
        float f = ((Amplitude+1.0)*0.5);
        gl_FragColor = vec4(vec3(ColorVar.r+F.x, ColorVar.g+F.y,  ColorVar.b+0.1*f), 0.1);

        
    } else if (Option == 2.0) {        
    
        // bubble_rings_ver1
        vec2 F = cellular(st * 50.0);
        float rings = 1.0 - aastep(0.45, F.y) + aastep(0.55, F.x);
        gl_FragColor = vec4(vec3(rings), 1.0);
      
        
    } else if (Option == 3.0) {          
        
        // bubble_rings_ver2
        
        // make grid of rings
        pattern = smoothstep(0.32, ((Amplitude+1.0)*0.5)*0.25, length(fract(6.0*st) - 0.5));
        
        // draw underlying texture
        vec2 G = cellular(st * 20.0);
        float rings = 1.0 - aastep(0.45, G.y) + aastep(0.55, G.x);

        vec2 F = cellular(st * 50.0);    
        float f = 1.0; // from 0 to 1
        gl_FragColor = vec4(vec3(F*pattern, 0.1*f), 0.1);
        

    } else if (Option == 4.0) {          
        
        // bubble_rings_ver3
        
        // make grid of rings
        pattern = smoothstep(0.32, ((Amplitude+1.0)*0.5)*0.25, length(fract(NRings*st) - 0.5));
        
        // draw underlying texture
        vec2 G = cellular(st * 20.0);
        float rings = 1.0 - aastep(0.45, G.y) + aastep(0.55, G.x);
        
        vec2 F = cellular(st * 5.0*Scale);    
        float f = ((Amplitude+1.0)*0.5);
        gl_FragColor = vec4(vec3(ColorVar.r+F.x*G.x, ColorVar.g+F.y*G.y,  ColorVar.b+0.1*f)*pattern, 0.1);

    
    } else if (Option == 5.0) {
    
        // worley_blobs_ver1
        // cellular 3 - blobs
        vec2 F = cellular(st * 30.0);
        float blobs = 1.0 - F.x*F.x;
        gl_FragColor = vec4(vec3(blobs), 1.0);

        // worley_blobs_ver2
//        vec2 F = cellular(st * 30.0);
//        float blobs = 1.0 - F.y*F.y*F.x*Shape;
//        gl_FragColor = vec4(vec3(blobs), 1.0);

        // worley_blobs_ver3
//        vec2 F = cellular(st * 50.0);
//        float blobs = 1.0 - pow(F.x, 3.0)*F.y*Shape;
//        gl_FragColor = vec4(vec3(blobs), 1.0);
        
        // worley_blobs_ver4 (color)
//        vec2 F = cellular(st * 30.0);
//        float blobs = 1.0 - F.y*F.y*F.x*Shape;
//        gl_FragColor = vec4(vec3(ColorVar.r*blobs, ColorVar.g*blobs, ColorVar.b*blobs), 1.0);        

    
    } else if (Option == 6.0) { 
    
        // cellular 4
//        vec2 F = cellular(st * 20.0);    
//        float facets = Amplitude + (F.y - F.x);
//        gl_FragColor = vec4(vec3(facets), 1.0);
        
        // worley_facets_ver1
        vec2 F = cellular(vec2(s, t * 40.0));    
        float facets = 0.1 + (F.y - F.x);
        gl_FragColor = vec4(vec3(facets), 1.0);
        

    } else if (Option == 7.0) { 

        // worley_cell_ver4
        vec2 F = cellular(st * 30.0);
        float blobs = 1.0 - F.y*F.y*F.x*Shape;
        
        vec2 G = cellular(vec2(s, t * 40.0));    
        float facets = Depth + (F.x - F.y);
        gl_FragColor = vec4(vec3(ColorVar.r+blobs*facets, 0.2*ColorVar.g+blobs*facets, 0.3*ColorVar.b+blobs*facets), 1.0);

    } else {
    
        // worley_blotches_ver1
        float d = length(fract(st*NRings) - 0.5);
        float n = snoise(vec3(12.0*st, 0.0)) + 0.5*snoise(vec3(Blotchyness*11.0*st, 2.0));
        float blotches = 1.0 - aastep(0.23, d + 0.1*n);
                
        // draw underlying texture
        vec2 G = cellular(st * 10.0);
        float rings = 1.0 - aastep(0.45, G.y) + aastep(0.55, G.x);
        
        vec2 F = cellular(st * 5.0*Scale);    
        float f = ((Amplitude+1.0)*0.5);
        gl_FragColor = vec4(vec3(ColorVar.r+F.x*G.x, ColorVar.g+F.y*G.y,  ColorVar.b+0.1*f)*blotches, 0.1);
    
    }
    
}





// Cellular noise ("Worley noise") in 2D in GLSL.
// Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
// This code is released under the conditions of the MIT license.
// See LICENSE file for details.

// Permutation polynomial: (34x^2 + x) mod 289
vec3 permute(vec3 x) {
    return mod((34.0 * x + 1.0) * x, 289.0);
}

// Cellular noise, returning F1 and F2 in a vec2.
// Standard 3x3 search window for good F1 and F2 values

vec2 cellular(vec2 P) {
#define K 0.142857142857 // 1/7
#define Ko 0.428571428571 // 3/7
#define jitter 1.0 // Less gives more regular pattern
	vec2 Pi = mod(floor(P), 289.0);
 	vec2 Pf = fract(P);
	vec3 oi = vec3(-1.0, 0.0, 1.0);
	vec3 of = vec3(-0.5, 0.5, 1.5);
	vec3 px = permute(Pi.x + oi);
	vec3 p = permute(px.x + Pi.y + oi); // p11, p12, p13
	vec3 ox = fract(p*K) - Ko;
	vec3 oy = mod(floor(p*K),7.0)*K - Ko;
	vec3 dx = Pf.x + 0.5 + jitter*ox;
	vec3 dy = Pf.y - of + jitter*oy;
	vec3 d1 = dx * dx + dy * dy; // d11, d12 and d13, squared
	p = permute(px.y + Pi.y + oi); // p21, p22, p23
	ox = fract(p*K) - Ko;
	oy = mod(floor(p*K),7.0)*K - Ko;
	dx = Pf.x - 0.5 + jitter*ox;
	dy = Pf.y - of + jitter*oy;
	vec3 d2 = dx * dx + dy * dy; // d21, d22 and d23, squared
	p = permute(px.z + Pi.y + oi); // p31, p32, p33
	ox = fract(p*K) - Ko;
	oy = mod(floor(p*K),7.0)*K - Ko;
	dx = Pf.x - 1.5 + jitter*ox;
	dy = Pf.y - of + jitter*oy;
	vec3 d3 = dx * dx + dy * dy; // d31, d32 and d33, squared
	// Sort out the two smallest distances (F1, F2)
	vec3 d1a = min(d1, d2);
	d2 = max(d1, d2); // Swap to keep candidates for F2
	d2 = min(d2, d3); // neither F1 nor F2 are now in d3
	d1 = min(d1a, d2); // F1 is now in d1
	d2 = max(d1a, d2); // Swap to keep candidates for F2
	d1.xy = (d1.x < d1.y) ? d1.xy : d1.yx; // Swap if smaller
	d1.xz = (d1.x < d1.z) ? d1.xz : d1.zx; // F1 is in d1.x
	d1.yz = min(d1.yz, d2.yz); // F2 is now not in d2.yz
	d1.y = min(d1.y, d1.z); // nor in  d1.z
	d1.y = min(d1.y, d2.x); // F2 is in d1.y, we're done.
	return sqrt(d1.xy);
}


// Replacement for RSL's 'filterstep()', with fwidth() done right.
// 'threshold ' is constant , 'value ' is smoothly varying
float aastep (float threshold , float value) {
    float afwidth = 0.7 * length ( vec2(dFdx(value), dFdy(value)));
    // GLSL 's fwidth(value) is abs(dFdx(value)) + abs(dFdy(value))
    return smoothstep (threshold-afwidth, threshold+afwidth, value );
}




// Simplex noise
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{ 
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
    
    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;
    
    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );
    
    //   x0 = x0 - 0.0 + 0.0 * C.xxx;
    //   x1 = x0 - i1  + 1.0 * C.xxx;
    //   x2 = x0 - i2  + 2.0 * C.xxx;
    //   x3 = x0 - 1.0 + 3.0 * C.xxx;
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
    vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y
    
    // Permutations
    i = mod289(i); 
    vec4 p = permute( permute( permute( 
                                       i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                              + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
                     + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
    
    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float n_ = 0.142857142857; // 1.0/7.0
    vec3  ns = n_ * D.wyz - D.xzx;
    
    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)
    
    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)
    
    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);
    
    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );
    
    //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
    //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));
    
    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
    
    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);
    
    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    
    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                 dot(p2,x2), dot(p3,x3) ) );
}
#include <VEC3F.h>
#include "OBJ.h"

//////////////////////////////////////////////////////////////////////
// Write out a PPM file of dimensions width x height to 'filename'
//////////////////////////////////////////////////////////////////////
void writePPM(VEC3F* image, int width, int height, const char* filename)
{
  // Dump to PPM
  FILE *f = fopen(filename, "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      int i = x + (height - 1 - y) * width;

      // map to the [0,255] range
      int red   = image[i][0] * 255;
      int green = image[i][1] * 255;
      int blue  = image[i][2] * 255;
      fprintf(f,"%d %d %d ", red, green, blue);
    }
}

//////////////////////////////////////////////////////////////////////
// Create an image and write it out to a file
//////////////////////////////////////////////////////////////////////
void renderFrame()
{
  int width  = 100;
  int height = 100;
  VEC3F* image = new VEC3F[width * height];

  VEC3F eye(0,0,-1);

  // load an transform the bunny
  OBJ bunny;
  bunny.load("bunny_small.obj");
  bunny.normalize();
  VEC3F translation(0,0,2);
  bunny.translate(translation);

  // get the mesh triangles
  std::vector<TRIANGLE>& triangles = bunny.triangles;

  // set up a light
  VEC3F lightPosition(0,4,-2);

  // shoot out some rays
  int i = 0;
  float dx = 1.0 / width;
  float dy = 1.0 / height; 
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++, i++)
    {
      float xComponent = -0.5 + 0.5 * dx + x * dx;
      float yComponent = -0.5 + 0.5 * dy + y * dy;

      VEC3F origin(xComponent, yComponent, 0);
      VEC3F direction = origin - eye;
      direction.normalize();

      RAY ray(origin, direction);
      VEC3F normal;

      // loop through all the triangles, find the closest one
      float closest = 1e20;
      for (unsigned int z = 0; z < triangles.size(); z++)
      {
        float triDistance = triangles[z].intersect(ray, normal);
		
        if (triDistance > 0.0 && triDistance < closest)
        {
          VEC3F hit = ray.origin + triDistance * ray.direction;
          VEC3F light = lightPosition - hit;
          light.normalize();

          float dot = normal * light;
          if (dot < 0) dot = 0;
          image[i] = dot;

          closest = triDistance;
        }
      }
    }

  writePPM(image, width, height, "image.ppm");
  delete[] image;
}

//////////////////////////////////////////////////////////////////////
// Create an image and write it out to a file
//////////////////////////////////////////////////////////////////////
int main()
{
  renderFrame();

  return 0;
}

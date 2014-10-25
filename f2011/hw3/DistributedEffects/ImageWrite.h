#ifndef HW2_OO_ImageWrite_h
#define HW2_OO_ImageWrite_h

class ImageWrite {    
    
public:
    
    ImageWrite(){}
    
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
                // cout << "["<< red << ", " << green << ", " << blue << "]" << endl; 
            }
    }
};

#endif

#include "grey.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
void grey(const char* input, const char *output_jpg, const char *output_png){
    
    
    
    int width, height, bpp;

    uint8_t* rgb_image = stbi_load(input, &width, &height, &bpp, 0);

    
    printf("width is :%d, height is :%d, bpp is %d\n",width,height,bpp);
    uint8_t* new_image = malloc(sizeof(uint8_t) * (width*height));
    uint8_t* p = rgb_image;
    uint8_t* q = new_image;
    for (p,q; p != rgb_image + (width*height*bpp); p += bpp, q += 1){
        uint8_t red = *(p);
        uint8_t green = *(p+1);
        uint8_t blue = *(p+2);
        
        uint8_t grey = (red / 3) + (green / 3) + (blue/3);
        *(q) = grey;
    }
    //const char* result_jpg = sprintf("%s_result.jpg",argv[1]);
   // const char* result_png = sprintf("%s_result.png",argv[1]);
    stbi_write_jpg(output_jpg,width,height,1,(void*)new_image,100);
    stbi_write_png(output_png,width,height,1,(void*)new_image,width);
    

}
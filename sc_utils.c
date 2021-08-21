#include "sc_utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

void write_image(const char* output, int width, int height,int comp,uint8_t* data, int stride){
    stbi_write_png(output,width,height,comp,(void*)data,width);
}

uint8_t* read_image(const char* input, int *width_ptr, int *height_ptr,int* bpp_ptr){
    return stbi_load(input,width_ptr,height_ptr,bpp_ptr,0);
}


void grey(const char* input, const char *output){
    
    
    
    int width, height, bpp;

    
    uint8_t* rgb_image = read_image(input, &width, &height, &bpp);

    
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
    write_image(output,width,height,1,(void*)new_image,width);
    

}

void energy()
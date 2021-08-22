#include "sc_utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

void write_image(const char* output, int width, int height,int comp,uint8_t* data, int stride){
    stbi_write_png(output,width,height,comp,(void*)data,stride);
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

void energy(const char* input, const char* output){
        
    
    int width, height, bpp;
    uint8_t* rgb_image = read_image(input, &width, &height, &bpp);
    uint8_t* new_imagey = malloc(sizeof(uint8_t) *(width*height));
    
    int prev;
    int curr;
    int i;

    int **arr = (int **)malloc(height * sizeof(int *));
    if (arr == NULL) printf("arr is null\n");
    for (i=0; i<height; i++){
        arr[i] = (int *)malloc(width * sizeof(int));
        if (arr[i] == NULL) printf("inside is null\n");
    }
    
    int **outx = (int **)malloc(height * sizeof(int *));
    if (outx == NULL) printf("arrx is null\n");
    for (i=0; i<height; i++){
        outx[i] = (int *)malloc(width * sizeof(int));
        if (outx[i] == NULL) printf("inside is null\n");
    }
    int **outy = (int **)malloc(height * sizeof(int *));
    if (outy == NULL) printf("outy is null\n");
    for (i=0; i<height; i++){
        outy[i] = (int *)malloc(width * sizeof(int));
        if (outy[i] == NULL) printf("inside is null\n");
    }
    int **kernelx = (int **)malloc(3 * sizeof(int *));
    for (i = 0;i < 3; i ++)
        kernelx[i] = (int*)malloc(3*sizeof(int));
    
    int **kernely = (int **)malloc(3 * sizeof(int *));
    for (i = 0;i < 3; i ++)
        kernely[i] = (int*)malloc(3*sizeof(int));
    kernelx[0][0] = 1;
    kernelx[0][1] = 0;
    kernelx[0][2] = -1;
    kernelx[1][0] = 2;
    kernelx[1][1] = 0;
    kernelx[1][2] = -2;
    kernelx[2][0] = 1;
    kernelx[2][1] = 0;
    kernelx[2][2] = -1;
    
    kernely[0][0] = 1;
    kernely[0][1] = 2;
    kernely[0][2] = 1;
    kernely[1][0] = 0;
    kernely[1][1] = 0;
    kernely[1][2] = 0;
    kernely[2][0] = -1;
    kernely[2][1] = -2;
    kernely[2][2] = -1;
    
    int rows = height;
    int cols = width;
    int kCols = 3;
    int kRows = 3;
    
    int kCenterX = kCols / 2;
    int kCenterY = kRows / 2;
    for (int i = 0; i < height; i ++){
        for (int j = 0; j < width; j ++){
            arr[i][j] = *(rgb_image + (i*width)+j);
        }
    }
    for(int i=0; i < rows; ++i)              // rows
    {
        for(int j=0; j < cols; ++j)          // columns
        {
            for(int m=0; m < kRows; ++m)     // kernel rows
            {
                int mm = kRows - 1 - m;      // row index of flipped kernel

                for(int n=0; n < kCols; ++n) // kernel columns
                {
                    int nn = kCols - 1 - n;  // column index of flipped kernel

                    // index of input signal, used for checking boundary
                    int ii = i + (kCenterY - mm);
                    int jj = j + (kCenterX - nn);

                    // ignore input samples which are out of bound
                    if( ii >= 0 && ii < rows && jj >= 0 && jj < cols ){
                        //printf("i:%d j:%d ii:%d jj:%d mm:%d nn:%d\n",i,j,ii,jj,mm,nn);
                        outx[i][j] += arr[ii][jj] * kernelx[mm][nn];
                        outy[i][j] += arr[ii][jj] * kernely[mm][nn];
                    }
                }
            }
        }
    }
    
    
    for (int i = 0; i < height; i ++){
        for (int j = 0; j < width; j ++){
            //*(new_imagex + (i*width) + j) = outx[i][j];
            *(new_imagey + (i*width) + j) = (int) sqrt(pow(outy[i][j],2)+pow(outx[i][j],2));
        }
    }
    
    write_image(output,width,height,1,(void*)new_imagey,width);
    

}
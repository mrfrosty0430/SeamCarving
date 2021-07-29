#include <stdio.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "math.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define MAX_SEAM 5


int main(int argc, char *argv[]){
    
    
    
    int width, height, bpp;
    uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &bpp, 0);
    printf("width: %d, height: %d, bpp: %d\n",width,height,bpp);
    uint8_t* new_image = malloc(sizeof(uint8_t) *(width*height));
    
    int **arr = (int **)malloc(height * sizeof(int *));
    if (arr == NULL) printf("arr is null\n");
    for (int i=0; i<height; i++){
        arr[i] = (int *)malloc(width * sizeof(int));
        if (arr[i] == NULL) printf("inside is null\n");
    }
    
    int **dp = (int **)malloc(height * sizeof(int *));
    if (dp == NULL) printf("arr is null\n");
    for (int i=0; i<height; i++){
        dp[i] = (int *)malloc(width * sizeof(int));
        if (dp[i] == NULL) printf("inside is null\n");
    }
    
    for (int i = 0; i < height; i ++){
        for (int j = 0; j < width; j ++){
            arr[i][j] = *(rgb_image + (i*width)+j);
            dp[i][j] = *(rgb_image + (i*width)+j);
        }
    }
    
    
    
    for (int seam = 0; seam < MAX_SEAM;seam ++){
        
        for (int i = 1; i < height; i ++){
            for (int j = 0; j < width; j ++){
                int currmin = dp[i-1][j];
                if (j-1 >= 0) currmin = min(currmin,dp[i-1][j-1]);
                if (j+1 < width) currmin = min(currmin,dp[i-1][j+1]);
                dp[i][j] = currmin + arr[i][j];
            }
        }
        
        
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
    
    
    

    //stbi_write_png(argv[2],width,height,1,(void*)new_imagex,width);
    stbi_write_png(argv[2],width,height,1,(void*)new_image,width);
    

}
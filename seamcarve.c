#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif



#include "sc_utils.h"


struct Node {
    int index;
    struct Node* next;
};



struct pixel{
    int red; 
    int blue;
    int green;
};

typedef struct pixel pixel_t;





int main(int argc, char *argv[]){
    
    //int width, height, bpp;
    //uint8_t* rgb_image = read_image(argv[1], &width, &height, &bpp);
    //printf("width: %d, height: %d, bpp: %d\n",width,height,bpp);
    const char* input = argv[1];
    const char* input_grey = argv[2];
    const char* input_energy = argv[3];
    const char* output = argv[4];
    int MAX_SEAM = atoi(argv[5]);
    grey(input, input_grey);
    energy(input_grey,input_energy);

    int width, height, bpp;
    uint8_t* energy_image = read_image(input_energy, &width, &height, &bpp);
    uint8_t* orig_image = read_image(input,&width,&height,&bpp);

    pixel_t **arr = (pixel_t **)malloc(height * sizeof(pixel_t *));
    if (arr == NULL) printf("arr is null\n");
    for (int i=0; i<height; i++){
        arr[i] = (pixel_t *)malloc(width * sizeof(pixel_t));
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
            arr[i][j].red = *(orig_image + ((i*width)+j) * bpp );
            arr[i][j].green = *(orig_image + ((i*width)+j) * bpp + 1);
            arr[i][j].blue = *(orig_image + ((i*width)+j) * bpp + 2);
            //printf("%d:%d:%d\n",arr[i][j].red,arr[i][j].green,arr[i][j].blue);
            dp[i][j] = *(energy_image + (i*width)+j);
            //int k = (i * width) + j;
            //printf("orig %d:%d:%d\n",*(orig_image + (k * bpp) + 0),*(orig_image + (k * bpp) + 1),*(orig_image + (k * bpp) + 2));
        }
        //break;
    }
    /*
    for (int i =0; i < height*width; i ++){
        printf("orig %d:%d:%d\n",*(orig_image + (i * bpp) + 0),*(orig_image + (i * bpp) + 1),*(orig_image + (i * bpp) + 2));
        break;
    }
    */
    
    for (int seam = 0; seam < MAX_SEAM;seam ++){
        
        for (int i = 1; i < height; i ++){
            for (int j = 0; j < width-seam; j ++){
                int currmin = dp[i-1][j];
                if (j-1 >= 0) currmin = min(currmin,dp[i-1][j-1]);
                if (j+1 < width) currmin = min(currmin,dp[i-1][j+1]);
                dp[i][j] = currmin + dp[i][j];
            }
        }
    
        int start = dp[height-1][0];
        int startidx = 0;
        for (int j = 0; j < width-seam; j ++){
            if (dp[height-1][j] < start){
                start = min(start,dp[height-1][j]);
                startidx = j;
            }
        }

        struct Node* new = malloc(sizeof(struct Node));
        new -> index = startidx;
        struct Node* curr = new;
        for (int i = height - 2; i >= 0; i --){
            int prev_index = curr->index;
            int localmin = dp [i][prev_index];
            int minindex = prev_index;
            if (prev_index > 0 && localmin > dp[i][prev_index-1] && prev_index-1 < width - seam){
                localmin = dp[i][prev_index-1];
                minindex = prev_index-1;
            }
            if (prev_index < width -1 && localmin > dp[i][prev_index+1] && prev_index-1 < width - seam ){
                localmin = dp[i][prev_index+1];
                minindex = prev_index+1;
            }
            struct Node* temp = malloc(sizeof(struct Node));
            curr -> next = temp;
            temp->next = NULL;
            temp->index = minindex;
            curr = curr -> next;
        }
        curr = new;
        for (int i = height - 1; i >= 0; i --){
            for (int j = 0; j < width-seam ; j ++){
                if (j > curr -> index){
                    dp[i][j-1] = dp[i][j];
                    arr[i][j-1].red = arr[i][j].red;
                    arr[i][j-1].green = arr[i][j].green;
                    arr[i][j-1].blue = arr[i][j].blue;
                }
            }
            curr = curr->next;
        }
        curr = new;
        int i = 0;
        while(curr != NULL){
            i+= 1;
            struct Node* temp = curr -> next;
            free(curr);
            curr = temp;
            
        }
        printf("seam number is %d\n",seam);
    }

    uint8_t* new_image = malloc(sizeof(uint8_t) *((width-MAX_SEAM)*height)* bpp);
    for (int i = 0; i < height; i ++){
        for (int j = 0; j < width-MAX_SEAM; j ++){
            *(new_image + ((i*(width-MAX_SEAM)) + j)*bpp ) = arr[i][j].red;
            *(new_image + ((i*(width-MAX_SEAM)) + j)*bpp + 1) = arr[i][j].green;
            *(new_image + ((i*(width-MAX_SEAM)) + j)*bpp + 2) = arr[i][j].blue;
            
        }
    }
    printf("out of loop2\n");

    //stbi_write_png(argv[2],width,height,1,(void*)new_imagex,width);
    write_image(output,width-MAX_SEAM,height,bpp,new_image,(width-MAX_SEAM)*bpp);
    
    
}
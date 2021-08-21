#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define MAX_SEAM 10

#include "sc_utils.h"


struct Node {
    int index;
    struct Node* next;
};




int main(int argc, char *argv[]){
    
    //int width, height, bpp;
    //uint8_t* rgb_image = read_image(argv[1], &width, &height, &bpp);
    //printf("width: %d, height: %d, bpp: %d\n",width,height,bpp);
    const char* input = argv[1];
    const char* input_grey = argv[2];
    const char* output = argv[3];
    grey(input, input_grey);
    
    int width, height, bpp;
    uint8_t* grey_image = read_image(input_grey, &width, &height, &bpp);
    uint8_t* orig_image = read_image(input,&width,&height,&bpp);

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
            for (int j = 0; j < width-seam; j ++){
                int currmin = dp[i-1][j];
                if (j-1 >= 0) currmin = min(currmin,dp[i-1][j-1]);
                if (j+1 < width) currmin = min(currmin,dp[i-1][j+1]);
                dp[i][j] = currmin + arr[i][j];
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
                    arr[i][j-1] = arr[i][j];
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

    uint8_t* new_image = malloc(sizeof(uint8_t) *((width-MAX_SEAM)*height));
    for (int i = 0; i < height; i ++){
        for (int j = 0; j < width-MAX_SEAM; j ++){
            *(new_image + (i*(width-MAX_SEAM)) + j) = arr[i][j];
        }
    }
    printf("out of loop2\n");

    //stbi_write_png(argv[2],width,height,1,(void*)new_imagex,width);
    write_image(output,width-MAX_SEAM,height,1,new_image,(width-MAX_SEAM) * sizeof(uint8_t));
    
}
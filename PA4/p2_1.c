#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  FILE *numFile;
  numFile = fopen("number.txt","r");
  int nums[64];
  if (numFile == NULL){
    printf("Error Reading File\n");
    exit (0);
  }
  for (int i = 0; i < 64; i++){
    fscanf(numFile, "%d\n", &nums[i]);
  }
  for (int i = 0; i<64;i++){
    printf("Number %d: %d\n",i,nums[i]);
  }
}

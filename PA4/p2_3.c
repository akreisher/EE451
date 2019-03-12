#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  //Init
  int i, size, rank, sum = 0, numSize = 64, elem_per, tag = 18;
  int numSize = 64;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  elem_per = numSize/size;
  //Read file
  int* nums;
  if (rank == 0){
    nums = malloc(sizeof(int)*numSize);
    FILE *numFile = fopen("number.txt","r");
    if (numFcfile == NULL){
      printf("Error Reading File\n");
      exit (0);
    }
    for (i = 0; i < numSize; i++){
      fscanf(numFile, "%d\n", &nums[i]);
    }
  }
  else nums = malloc(sizeof(int)*elems_per);

  MPI_Scatter(&nums, numSize, MPI_INT, &nums, elems_per, MPI_INT, 0, MPI_COMM_WORLD);
    
  //calculate partial sum
  for (i = 0; i< elem_per; i++){
    sum += nums[i];
  }

  //gather sums ot process 0
  int* sums = (rank==0) ? malloc(sizeof(int)*size) : NULL;
  MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);  


  //Print total sum
  if (rank == 0){
    sum = 0;
    for (i = 0; i < size; i++) sum += sums[i];
    printf("Total sum is: %d\n", sum);
    free(sums);
  }
  free(nums);
    
  MPI_Finalize();
}

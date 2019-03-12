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
  int nums[numSize];
  FILE *numFile = fopen("number.txt","r");
  if (numFile == NULL){
    printf("Error Reading File\n");
    exit (0);
  }
  for (i = 0; i < numSize; i++){
    fscanf(numFile, "%d\n", &nums[i]);
  }

  //calcualte partial sum
  for (i = rank*elem_per; i<(rank+1)*elem_per;i++){
    sum += nums[i];
  }

  //Gather sum at process 0
  int* sums = (rank==0) ? malloc(sizeof(int)*size) : NULL;
  MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

  //Compute total sum
  if (rank == 0){
    sum = 0;
    for (i = 0; i < size; i++) sum += sums[0];
    printf("Total sum is: %d\n", sum);
    free(sums);
  }

  MPI_Finalize();
}

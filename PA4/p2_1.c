#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  //Init
  int i, size, rank, sum = 0, numSize = 64, elem_per, msg, tag = 18;
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

  //Compute total sum at process 0
  if (rank == 0){
    for (i = 1; i < size; i++){
      MPI_Recv(&msg,1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
      sum += msg;
    }
    printf("Total sum is: %d\n", sum);
  }
  else{
    MPI_Send(&sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}

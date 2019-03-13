#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  //Init
  int i, size, rank, sum = 0, total, numSize = 64, elem_per, tag = 18;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  elem_per = numSize/size;
  //Read file
  int nums[numSize];
  if (rank == 0){
    FILE *numFile = fopen("number.txt","r");
    if (numFile == NULL){
      printf("Error Reading File\n");
      exit (0);
    }
    for (i = 0; i < numSize; i++){
      fscanf(numFile, "%d\n", &nums[i]);
    }
  }

  MPI_Bcast(&nums, numSize, MPI_INT, 0, MPI_COMM_WORLD);
    
  //calculate partial sum
  for (i = rank*elem_per; i<(rank+1)*elem_per;i++){
    sum += nums[i];
  }

  //Reduce sum at process 0
  MPI_Reduce(&sum, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  //Print total sum
  if (rank == 0) printf("Total sum is: %d\n", total);
    
  MPI_Finalize();
}

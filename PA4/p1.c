#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  int i, size, rank,tag = 18;
  
  char message[20];
  MPI_Init(&argc,&argv);
  MPI_Comm_Size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;

  if (rank == 0){
    strcopy(message,"Hello, World!");
    for (i = 1; i<size; i++){
      MPI_Send(message,13,MPI_CHAR,i,tag,MPI_COMM_WORLD);
    }
  }
  else{
    MPI_Recv(message,20,MPI_CHAR,0,tag,MPI_COMM_WORLD, &status);
  }
  printf("Message from processor %d: %13s", rank, message);

  MPI_Finalize();
}
      

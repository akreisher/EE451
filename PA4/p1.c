#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]){
  int i, message, size, rank, dest, tag = 18;
  MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0){
    message = 451;
    dest = rank+1;
    printf("Process %d: Initially Msg = %d\n",rank,message);
    MPI_Send(&message,1,MPI_INT,dest,tag,MPI_COMM_WORLD);
    MPI_Recv(&message,1,MPI_INT,size-1,tag,MPI_COMM_WORLD,&status);
    printf("Process %d: Received  Msg = %d. Done!\n",rank,message);
  }
  else{
    MPI_Recv(&message, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
    message++;
    printf("Process %d: Msg = %d\n",rank,message);
    dest = rank + 1;
    if (dest >= size)dest = 0;
    MPI_Send(&message, 1, MPI_INT, dest, tag, MPI_COMM_WORLD); 
}
  MPI_Finalize();
  return 0;
}
      

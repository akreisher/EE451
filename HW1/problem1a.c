#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int argc, char*argv[]){          // this example is about multiplying a 4096*4096 matrix with a vector
  long n;
    int num_threads;
    if (argc  < 2){
      printf("Must supply at least number of threads\n");
      return 0;
    }
    else if (argc==2){
      num_threads = atoi(argv[1]);
      n = 4096;
    }
    else{
      num_threads = atoi(argv[1]);
      n = (long)atoi(argv[2]);
    }

  int i, j,k;
  struct timespec start, stop;
  double time;

  double **A = (double**) malloc (sizeof(double*)*n);
  double **B = (double**) malloc (sizeof(double*)*n);
  double **C = (double**) malloc (sizeof(double*)*n);
  for (i=0; i<n; i++) {
    A[i] = (double*) malloc(sizeof(double)*n);
    B[i] = (double*) malloc(sizeof(double)*n);
    C[i] = (double*) malloc(sizeof(double)*n);
  }
    
  for (i=0; i<n; i++){
    for(j=0; j< n; j++){
      A[i][j]=i;
      B[i][j]=i+j;
      C[i][j]=0;      
    }
  }
  if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) { perror( "clock gettime" );}

  omp_set_num_threads(num_threads);
  // matrix * vector
  #pragma omp parallel for
  for(i=0; i< n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
        C[i][j] += A[i][k] * B[k][j];


  if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror( "clock gettime" );}
  time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;

  printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);    
  printf("C[100][100]=%f\n", C[100][100]);
  // release memory

  for (i=0; i<n; i++){
  	free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);

  return 0;
}

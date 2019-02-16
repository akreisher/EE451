#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

struct thread_data{
	int tid, size, num_rows;
	double** M1,**M2,**RES;
};

void *worker (void* args){
	//Init
	int i,j,k,tid, n, num_rows;
	double **A,**B,**C;
	double sum;
	struct thread_data* tdata;

	//Parse thread data
	tdata = (struct thread_data*) args;
	tid = tdata->tid;
	n = tdata->size;
	num_rows = tdata->num_rows;
	A = tdata->M1;
	B = tdata->M2;
	C = tdata->RES;

	//Perform mult
	for (i = tid*num_rows;i<(tid+1)*num_rows;i++){
		for (j=0;j<n;j++){
			sum = 0;
			for (k=0;k<n;k++){
				sum += A[i][k]*B[k][j];
			}
			C[i][j]=sum;
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
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
		int i, j, k;
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
				
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//
		pthread_t * threads;
		struct thread_data* thread_data_array;
		int rc, num_rows;

		num_rows = n/num_threads;
		threads= (pthread_t*) malloc(sizeof(pthread_t)*num_threads);
		thread_data_array = (struct thread_data*) malloc(sizeof(struct thread_data)*num_threads);


		for (i = 0; i<num_threads;i++){
			thread_data_array[i].tid = i;
			thread_data_array[i].num_rows = num_rows;
			thread_data_array[i].size = (int)n;
			thread_data_array[i].M1 = A;
			thread_data_array[i].M2 = B;
			thread_data_array[i].RES =C;
			rc = pthread_create(&threads[i],NULL,worker,(void *) &thread_data_array[i]);
			if (rc) { printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);}
		}

		//Wait for results
		for (i=0;i<num_threads;i++){
			rc = pthread_join(threads[i],NULL);
			if (rc) { printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);}
		}
		
		//*******************************//
		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
		printf("C[100][100]=%f\n", C[100][100]);
		
		// release memory
		free(thread_data_array);
		free(threads);
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		return 0;
}

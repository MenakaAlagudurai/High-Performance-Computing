#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define N 10
int main(){
	int rank,size;
	int arr[N];
	int sub_arr[N];
	int number;
	int local_count=0,total_count=0;
	int elements_per_process;
	double start_time,end_time,exec_time;

	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	elements_per_process=N/size;

	if(rank==0){
		scanf("%d",&number);
		printf("Generated Array:");
		for(int i=0;i<N;i++){
			arr[i]=rand()%10;
			printf("%d\t",arr[i]);
		}
		printf("\n");
	}
	start_time=MPI_Wtime();

	MPI_Bcast(&number,1,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Scatter(arr,elements_per_process,MPI_INT,sub_arr,elements_per_process,MPI_INT,0,MPI_COMM_WORLD);

	printf("Process : %d\n",rank);

	for(int i=0;i<elements_per_process;i++){
		printf("%d\t",sub_arr[i]);
		if(sub_arr[i]==number){
			local_count++;
		}
	}
	printf("\n");

	MPI_Reduce(&local_count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	end_time=MPI_Wtime()-start_time;

	printf("Process: %d | Frequency of %d | Execution Time: %f seconds\n",rank,local_count,end_time);

	if(rank==0){
		printf("Total frequency of %d = %d\n",number,total_count);
	}

	MPI_Finalize();
	return 0;
}


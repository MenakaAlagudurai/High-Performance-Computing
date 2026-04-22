#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

#define MAX 100

int palindrome(char str[]){
    int len=strlen(str);
    for(int i=0;i<len/2;i++){
        if(str[i]!=str[len-i-1]){
            return 0;
        }
    }
    return 1;
}

int main(int argc,char *argv[]){
    int rank,size;
    char msg[MAX];
    MPI_Status status;
    float start_time,end_time;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    start_time=MPI_Wtime();

    if(rank!=0){
        if(rank==1){
		strcpy(msg,"hello");
	}else if(rank==2){
		strcpy(msg,"madam");
	}else if(rank==3){
		strcpy(msg,"menaka");
	}else if(rank==4){
		strcpy(msg,"malayalam");
	}
        if(rank%2==1){
            MPI_Send(msg,MAX,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else{
            MPI_Send(msg,MAX,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
    }
    else{
        for(int i=1;i<size;i++){
            MPI_Recv(msg,MAX,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            
            if(status.MPI_TAG==1){
                for(int k=0;msg[k]!='\0';k++){
                    msg[k]=toupper(msg[k]);
                }
                printf("Process %d | Tag 1 | Uppercase: %s",MPI_ANY_SOURCE,msg);
            }
            else if(status.MPI_TAG==2){
                if(palindrome(msg)){
                    printf("Process %d | Tag 2 | %s is a palindrome",MPI_ANY_SOURCE,msg);
                }
                else{
                    printf("Process %d | Tag 2 | %s is not a palindrome",MPI_ANY_SOURCE,msg);
                }

            }
            
            end_time=MPI_Wtime()-start_time;
            printf(" | Exec. time: %lf milliseconds\n",end_time*1000);
        }
    }

    MPI_Finalize();
    return 0;
}

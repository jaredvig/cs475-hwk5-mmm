#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

int main(int argc, char *argv[]) {
	double clockstart, clockend;
	if(argc<3||argc>4){
		printf("Error, too many or too few arguments given, please give the proper amount of arguments.\n");
		exit(1);
	} 
	if(strcmp(argv[1],"P") !=0 && strcmp(argv[1],"S")!=0){
		printf("Error, please give a correct argument for either sequential (S) or parallel (P)\n");
		exit(1);
	}
	//PARALLELIZED
	if(strcmp(argv[1],"P")==0){
		int length = atoi(argv[3]);
		int threads = atoi(argv[2]);
		if(threads>length){
			printf("Error, the number of threads is larger than the size of the matrix.\n");
			exit(1);
		}
		if(threads<0){
		printf("Error, number of threads cannot be negative.\n");
		exit(1);
		}
		if(length<0){
		printf("Error, size of matrix cannot be negative.\n");
		exit(1);
		}
		printf("==============\n");
		printf("mode: parallel\n");
		printf("thread count: %d\n",threads);
		printf("size:%d\n", length);
		printf("==============\n");
		mmm_init(length);
		pthread_t* tid = (pthread_t*)malloc(threads*sizeof(pthread_t));
		for(int i = 0;i<threads;i++){
			int* arguments = (int*)malloc(2*sizeof(int));
			arguments[1] = threads;
			arguments[0] = i;
			//pthread_t thread;
			//pthread_create(&thread,NULL,mmm_par,arguments);
			pthread_create(&tid[i],NULL,mmm_par,arguments);
		}
		for(int i = 0;i<threads;i++){
		pthread_join(tid[i],NULL);
		}
		free(tid);

		clockstart = rtclock();
		for(int i =0;i<3;i++){
			pthread_t* tid = (pthread_t*)malloc(threads*sizeof(pthread_t));
			for(int j = 0;j<threads;j++){
				int* arguments = (int*)malloc(2*sizeof(int));
				arguments[1] = threads;
				arguments[0] = j;
				pthread_t thread;
				pthread_create(&thread,NULL,mmm_par,arguments);
				tid[j] = thread;
			}
			for(int j = 0;j<threads;j++){
			pthread_join(tid[j],NULL);
			}
			free(tid);
		}
		clockend = rtclock();
		double totalTime= clockend-clockstart;
		double avgTime = totalTime/3;
		printf("Parallel Time (avg of 3 runs): %f sec\n",avgTime);
		
		mmm_seq();
		clockstart = rtclock();
		for(int i =0;i<3;i++){
		mmm_seq();
		}
		clockend = rtclock();
		totalTime= clockend-clockstart;
		double avgTime2 = totalTime/3;
		double speedup = avgTime2/avgTime;
		printf("Sequential Time (avg of 3 runs): %f sec\n",avgTime2);
		printf("Speedup: %f\n",speedup);
		printf("Verifying...");
		double largestError = mmm_verify();
		printf("largest error between parallel and sequential matrix:%f\n",largestError);
	} 
	if(strcmp(argv[1],"S")==0){
		int size = atoi(argv[2]);
		if(size<0){
		printf("Error, size of matrix cannot be negative.\n");
		exit(1);
		}
		printf("==================\n");
		printf("mode: sequential\n");
		printf("thread count: 1\n");
		printf("size:%d\n", size);
		printf("==================\n");
		mmm_init(size);
		mmm_seq();
		clockstart = rtclock();
		for(int i =0;i<3;i++){
		mmm_seq();
		}
		clockend = rtclock();
		double totalTime= clockend-clockstart;
		double avgTime = totalTime/3;
		printf("Sequential Time (avg of 3 runs): %f sec\n",avgTime);
	}
	mmm_freeup();
	// start: stuff I want to clock

	// end: stuff I want to clock

	//clockend = rtclock(); // stop clocking
	//printf("Time taken: %.6f sec\n", (clockend - clockstart));

	return 0;
}

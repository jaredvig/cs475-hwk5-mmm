#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "mmm.h"
#include<pthread.h>

double ** inputMatrix1;
double ** inputMatrix2;
double ** outputMatrixSeq;
double ** outputMatrixPar;
int globalSize;
/**
 * Allocate and initialize the matrices on the heap. Populate
 * the input matrices with random integers from 0 to 99
 */
void mmm_init(int size) {
	globalSize=size;
	initMatrices();
	mmmPopulateMatrices();
}
void initMatrices(){
	inputMatrix1 = (double**)calloc(globalSize,sizeof(double*));
	inputMatrix2 = (double**)calloc(globalSize,sizeof(double*));
	outputMatrixSeq = (double**)calloc(globalSize,sizeof(double*));
	outputMatrixPar = (double**)calloc(globalSize,sizeof(double*));
	for(int i = 0; i<globalSize;i++){
		inputMatrix1[i] = (double*)calloc(globalSize,sizeof(double));
		inputMatrix2[i] = (double*)calloc(globalSize,sizeof(double));
		outputMatrixSeq[i] = (double*)calloc(globalSize,sizeof(double));
		outputMatrixPar[i] = (double*)calloc(globalSize,sizeof(double));
	}
}

/**
 * Reset a given matrix to zeroes
 * @param matrix pointer to a 2D array
 */
void mmm_reset(double **matrix) {
	// TODO
	for(int i = 0; i<globalSize; i++){
		for(int j =0;j<globalSize; j++){
			matrix[i][j] = 0;
		}
	}
}
void mmmPopulateMatrices(){
	for(int i = 0; i<globalSize; i++){
		for(int j = 0; j<globalSize; j++){
		inputMatrix1[i][j] = (double)(rand()%100);
		inputMatrix2[i][j] = (double)(rand()%100);
	}
	}
}

/**
 * Free up memory allocated to all matrices
 */
void mmm_freeup() {
	for(int i = 0; i<globalSize;i++){
		free(inputMatrix1[i]);
		free(inputMatrix2[i]);
		free(outputMatrixSeq[i]);
		free(outputMatrixPar[i]);

		inputMatrix1[i] = NULL;
		inputMatrix1[i] = NULL;
		inputMatrix1[i] = NULL;
		inputMatrix1[i] = NULL;
	}
	free(inputMatrix1);
	free(inputMatrix2);
	free(outputMatrixSeq);
	free(outputMatrixPar);
	
	inputMatrix1 = NULL;
	inputMatrix1 = NULL;
	outputMatrixSeq = NULL;
	outputMatrixPar = NULL;


}

/**
 * Sequential MMM
 */
void mmm_seq() {
	// TODO - code to perform sequential MMM
	for(int i =0;i<globalSize;i++){
		for(int j =0;j<globalSize;j++){
			int outputCellValue = 0;
			for(int k = 0; k<globalSize;k++){
				outputCellValue+= inputMatrix1[i][k]*inputMatrix2[k][j];
			}
			outputMatrixSeq[i][j] = outputCellValue;
		}
	}
}

/**
 * Parallel MMM
 */
void *mmm_par(void *args) {
	int*input = (int*)args;
	int startRow = input[0];
	int threads = input[1];
	for(int i = 0; i<globalSize;i++){
		for(int j = startRow;j<globalSize;j+=threads){
			int outputCellValue =0;
			for(int k = 0;k<globalSize;k++){
				outputCellValue+= inputMatrix1[i][k]*inputMatrix2[k][j];
			}
			outputMatrixPar[i][j] = outputCellValue;
		}
	}
	 free(args);
	 args=NULL;
	return NULL;

	// TODO - code to perform parallel MMM

}

/**
 * Verifies the correctness between the matrices generated by
 * the sequential run and the parallel run.
 *
 * @return the largest error between two corresponding elements
 * in the result matrices
 */
double mmm_verify() {
	double largestDifference = 0;
	for(int i = 0; i <globalSize; i++){
		for(int j = 0;j <globalSize;j++){
			//printf("%f",outputMatrixPar[i][j]);
			//printf(" %f ",outputMatrixPar[i][j]);
			double difference = abs(outputMatrixPar[i][j] - outputMatrixSeq[i][j]);
			if(difference>largestDifference){
				largestDifference = difference;
			}
			}
		}
		
		return largestDifference;
	}


#ifndef MMM_H_
#define MMM_H_

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., extern double **A, **B, **C),
// as well as the size of the matrices, etc.

void mmm_init(int size);
void mmm_reset(double ** matrix);
void mmm_freeup();
void mmm_seq();
void *mmm_par(void *args);
double mmm_verify();
void mmmPopulateMatrices();
void initMatrices();
void freeMatrix(double ** matrix);
#endif /* MMM_H_ */

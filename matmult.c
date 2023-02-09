#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]
// intialize struct that will be sended as attribute to threads fuction 
struct attributes{
	int* A;
	int* B;
	int* C;
	int l;
	int m;
	int n;
    int i;
	int j;
};
/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */

void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
 
 void * c_mult (void * info)
 {
    // convert void pointer to struct pointer
    struct attributes *info_ptr = (struct attributes*) info;
          int sum = 0;
            // calculate 1 item in C
            for(int k=0; k< info_ptr->m; k++)  
                sum += Item(info_ptr->A, info_ptr->i, k, info_ptr->m) * Item(info_ptr->B, k, info_ptr->j, info_ptr->n);
            Item(info_ptr->C, info_ptr->i, info_ptr->j, info_ptr->n) = sum;
             
 }
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t threads[l*n];
    int  iret, num = 0;
    struct attributes informations[l*n];
    // loop over items of o/p matrix C and create threads = no. of items in C
    for(int i=0; i<l; i++) 
    {
        for(int j=0; j<n; j++)
        {
            // assign all values needed to struct to be sended as attributs of function that calculate o/p item in C
            informations[num].A = A;
            informations[num].B = B;
            informations[num].C = C;
            informations[num].l = l;
            informations[num].m = m;
            informations[num].n = n;
            informations[num].i = i;
            informations[num].j = j;
            
            iret = pthread_create(&threads[num], NULL, c_mult, &informations[num]);
             ++num;
        }
        
     }
    // wait untill all threads to finish then terminate to print the o/p matrix
    for (int i=0; i<num; i++){	
		pthread_join(threads[i], NULL);
	}
	
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 * 
 */

void * c_mult_v2 (void * info)
 {
    struct attributes *info_ptr = (struct attributes*) info;
    //calculate all items in specific row
    for(int j=0; j<info_ptr->n; j++)  
        {
          int sum = 0;
            for(int k=0; k< info_ptr->m; k++)
                sum += Item(info_ptr->A, info_ptr->i, k, info_ptr->m) * Item(info_ptr->B, k, j, info_ptr->n);
            Item(info_ptr->C, info_ptr->i, j, info_ptr->n) = sum;
                
        }
       
 }
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t threads[l*n];
    int  iret, num = 0;
    struct attributes informations[l*n];
    // create threads = no. of rows in o/p matrix C
    for(int i=0; i<l; i++)
        {
            informations[num].A = A;
            informations[num].B = B;
            informations[num].C = C;
            informations[num].l = l;
            informations[num].m = m;
            informations[num].n = n;
            informations[num].i = i;
            iret = pthread_create(&threads[num], NULL, c_mult_v2, &informations[num]);
             ++num;
        }

    for (int i=0; i<num; i++){	
		pthread_join(threads[i], NULL);
	}

}

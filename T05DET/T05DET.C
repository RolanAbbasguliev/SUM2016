/* File Name: T05DET
 * Programmer: RA3
 * Date: 06.06.2016 // Pushkin's birthday //
 */

#include <stdio.h>
#include <dos.h>
#define N 5
#define MAX 30

double A[MAX][MAX], sum;

int P[MAX], Parity = 0;


     for (j = 0; j < N; j++)
       fscanf(F, "%lf", &A[i][j]);
   fclose(F);
}
/* End of 'LoadMatrix' function */

/* Start of 'EvalDeterminant' function */
double EvalDeterminant( char *FileName )
{
   int i;

   LoadMatrix(FileName);
   sum = 0;
   for (i = 0; i < N; i++)
     P[i] = i;
   Go(0);
   return sum;
}
/* End of 'EvalDeterminant' function */


/* Start of 'Main' function */
void main( void )
{
   int i;
   char *M[]=
   {
     "m1.txt"
   };
   for (i = 0; i < sizeof(M) / sizeof(M[0]); i++)
     printf("Det(%d)[%s] = %f\n", i, M[i], EvalDeterminant(M[i]));
   getch();
}
/* End of 'Main' fuction */

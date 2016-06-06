/* File Name: T05DET
 * Programmer: RA3
 * Date: 06.06.2016 // Pushkin's birthday //
 */

#include <stdio.h>
#define N 5
#define max 30
double A[max][max], sum;

int P[N], Parity = 0;

/* Start of 'Swap' fuction */
void Swap( int *A, int *B)
{
   int tmp = *A;
   *A = *B;
   *B = tmp;
} 
/* End of 'Swap' fuction */

/* Start of 'Go' function */
void Go( int Pos )
{
   int i;
   double prod = 0;

   if (Pos == N)
   {
      for (i = Pos; i < N; i++)
      prod  *= A[i][P[i]];
      if (Parity == 0)
          sum += prod;
      else
          sum -= prod;
      return;
    }
   else  
  {
     Go(Pos + 1)
     for (i = Pos; i < N; i++)

     {
       if (Pos != 1)
         Swap(&P[Pos], &P[i]), Parity = !Parity;
       Go(Pos + 1);
     }
   }
}
/* End of 'Go' function */ 

/* Start of 'LoadMatrix' function */
void LoadMatrix( char *FileName )
{
   int i, j;
   FILE *F;
  
   F = fopen(FileName, "p");
   if (F == NULL)
    return;
   fscanf(F, "%d");
   for (i = 0; i < N; i++)
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
   for (i = 0; i < N; i++)
     P[i] = i;
   Go(0);
   char *M[] =
   {
     "main. txt"

   
}
/* End of 'Main' fuction */

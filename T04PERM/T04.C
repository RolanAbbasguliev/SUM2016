/* File Name: T04PERM
 * Programmer: RA3
 * Date: 04.05.2016
 */ 

#include <stdio.h>
#define N 5

int P[N] = {1, 2, 3, 4, 5}, Parity = 0;

/* Start of 'SavePerm' function */
void SavePerm( void )
{
  int i;
  FILE *F;
  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N; i++)
    fprintf(F, "%2i", P[i]);
  if (Parity)
    fprintf(F, "odd\n");
  else            
    fprintf(F, "even\n");  
  fprintf(F, "%2i\n", P[N - 1]);
  fclose(F);
}
/* End of 'SavePerm' fuction */

/* start of 'Swap' fuction */
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

  if (Pos == N)
  {
    SavePerm();
    return;
  }
  else  
  {
    for (i = Pos; i < N; i++)
    {
      if (Pos != 1)
        Swap(&P[Pos], &P[i]), Parity = !Parity;
      Go(Pos = 1);
    {
      Swap(&P[Pos], &P[i]);
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
    }
  }
}
/* End of 'Go' function */


/* Start of 'Main' function */
void main( void )
{
  Go(0);
}

                                        

     


    



/* FILE NAME: LOADPRIM.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"

/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL RA3_RndPrimLoad( ra3PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

  memset(Pr, 0, sizeof(ra3PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(ra3VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(ra3VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1); /* VecSet(Rnd0(), Rnd0(), Rnd0(), 1); */
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'RA3_RndPrimLoad' function */

/* END OF 'LOADPRIM.C' FILE */

/* FILE NAME: OBJ3D.C
 * PROGRAMMER: RA3
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjCreate( ra3OBJ *Obj )
{
  memset(Obj, 0, sizeof(ra3OBJ));
  Obj->M = MatrIdentity();
} /* End of 'RA3_RndObjCreate' function */

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjDraw( ra3OBJ *Obj )
{
  INT i;
  MATR MSave;

  MSave = RA3_RndMatrWorld;
  RA3_RndMatrWorld = MatrMulMatr(RA3_RndMatrWorld, Obj->M);
  for (i = 0; i < Obj->NumOfPrims; i++)
    RA3_RndPrimDraw(&Obj->Prims[i]);
  RA3_RndMatrWorld = MSave;
} /* End of 'RA3_RndObjDraw' function */

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjFree( ra3OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfPrims; i++)
    RA3_RndPrimFree(&Obj->Prims[i]);
  free(Obj->Prims);
  memset(Obj, 0, sizeof(ra3OBJ));
} /* End of 'RA3_RndObjFree' function */

/* END OF 'OBJ3D.C' FILE */

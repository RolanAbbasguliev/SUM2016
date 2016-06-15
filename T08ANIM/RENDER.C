/* FILE NAME: RENDER.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Render handle functions.
 */

#include "anim.h"

/* Global transformation matrices */
MATR
  RA3_RndMatrWorld, /* World (object) transformation matrix */
  RA3_RndMatrView,  /* Camera view transform */
  RA3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
FLT
  RA3_RndProjDist, /* Near clip plane */
  RA3_RndFarClip,  /* Far clip plane */
  RA3_RndProjSize; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_RndSetProj( VOID )
{
  FLT ratio_x = 1, ratio_y = 1;

  if (RA3_Anim.W >= RA3_Anim.H)
    ratio_x = (FLT)RA3_Anim.W / RA3_Anim.H;
  else
    ratio_y = (DBL)RA3_Anim.H / RA3_Anim.W;

  RA3_RndMatrProj = MatrFrustum(-ratio_x * RA3_RndProjSize / 2,
                                 ratio_x * RA3_RndProjSize / 2,
                                -ratio_y * RA3_RndProjSize / 2,
                                 ratio_y * RA3_RndProjSize / 2,
                                 RA3_RndProjDist, RA3_RndFarClip);
} /* End of 'RA3_RndSetProj' function */


VOID RA3_RndPrimDraw( ra3PRIM *Pr )
{
  INT i;
  MATR M;

  M = MatrMulMatr(RA3_RndMatrView, RA3_RndMatrProj);
  glLoadMatrixf(M.A[0]);
  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();

  /* Build transform matrix */
  M = MatrMulMatr(RA3_RndMatrWorld,
      MatrMulMatr(RA3_RndMatrView, RA3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'RA3_RndPrimDraw' function */

VOID RA3_RndPrimFree( ra3PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(ra3PRIM));
} /* End of 'RA3_RndPrimFree' function */

/* END OF 'RENDER.C' FILE */

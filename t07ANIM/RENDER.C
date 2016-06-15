/* FILE NAME: RENDER.C
 * PROGRAMMER: RA3
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include "anim.h"

/* Global transformation matrices */
MATR
  RA3_RndMatrWorld, /* World (object) transformation matrix */
  RA3_RndMatrView,  /* Camera view transform */
  RA3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
DBL
  RA3_RndProjDist, /* Near clip plane */
  RA3_RndFarClip,  /* Far clip plane */
  RA3_RndProjSize; /* Project plane size */

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (RA3_Anim.W >= RA3_Anim.H)
    ratio_x = (DBL)RA3_Anim.W / RA3_Anim.H;
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
  POINT *pts;

  /* Build transform matrix */
  M = MatrMulMatr(RA3_RndMatrWorld,
    MatrMulMatr(RA3_RndMatrView, RA3_RndMatrProj));

  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    /* NDC */
    VEC p = VecMulMatr(Pr->P[i], M);
    pts[i].x = (p.X + 1) * RA3_Anim.W / 2;
    pts[i].y = (-p.Y + 1) * RA3_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];

    MoveToEx(RA3_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(RA3_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
}
/* END OF 'RENDER.C' FILE */
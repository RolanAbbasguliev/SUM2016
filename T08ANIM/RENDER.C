/* FILE NAME: RENDER.C
 * PROGRAMMER: RA3
 * DATE: 13.06.2016
 * PURPOSE: Render handle functions.
 */

#include <stdio.h>
#include <string.h>

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

/* Shader support */
UINT RA3_RndPrg;

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

/* END OF 'RENDER.C' FILE */

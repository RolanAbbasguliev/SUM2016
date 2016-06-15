
/* FILE NAME: U_CONTRL.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Control unit sample.
 */

#include <time.h>

#include "anim.h"
#include "units.h"

/* Clock unit representation type */
typedef struct
{
  ra3UNIT; /* Base unit fields */
  VEC Pos;
} ra3UNIT_CONTROL;

static VOID RA3_UnitInit( ra3UNIT_CONTROL *Uni, ra3ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 3);
} /* End of 'RA3_UnitInit' function */


static VOID RA3_UnitResponse( ra3UNIT_CONTROL *Uni, ra3ANIM *Ani )
{
  DBL r;

  if (Ani->Keys['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['E'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani->Keys[VK_SPACE])
    RA3_AnimAddUnit(RA3_UnitCreateBall());
  if (Ani-> Keys['R'])
    RA3_AnimAddUnit(RA3_UnitCreateCube());
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    RA3_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    RA3_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateX(59 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(59 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(59 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateX(59 * Ani->Mdy * Ani->GlobalDeltaTime));
  }

  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(59 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(-59 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->DeltaTime * 0.1) / r);

  RA3_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'RA3_UnitResponse' function */


static VOID RA3_UnitRender( ra3UNIT_CONTROL *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitRender' function */




/* END OF 'U_CONTRL.C' FILE */

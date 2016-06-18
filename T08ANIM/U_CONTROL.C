
/* FILE NAME: U_CONTRL.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Control unit sample.
 */

#include <time.h>
#include <stdlib.h>
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

  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->KeysClick['E'])
    glPolygonMode(GL_BACK, GL_LINE);
  if (Ani-> KeysClick['X'])
    RA3_AnimAddUnit(RA3_UnitCreateMario());
  if (Ani->Keys[VK_RETURN] && Ani->Keys[VK_MENU])
    RA3_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    RA3_AnimDoExit();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['R'])
  {
    RA3_RndShaderFree(RA3_RndPrg);
    RA3_RndPrg = RA3_RndShaderLoad("a");
  }
  
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateX(80 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(80 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(80 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateX(80 * Ani->Mdy * Ani->GlobalDeltaTime));
  }

  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(80 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = VecMulMatr(Uni->Pos, MatrRotateY(-80 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + Ani->Mdz * Ani->GlobalDeltaTime * 2) / r);

  RA3_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'RA3_UnitResponse' function */

ra3UNIT * RA3_UnitCreateControl( VOID )
{
  ra3UNIT_CONTROL *Uni;

  if ((Uni = (ra3UNIT_CONTROL *)RA3_AnimUnitCreate(sizeof(ra3UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)RA3_UnitInit;

  Uni->Response = (VOID *)RA3_UnitResponse;
 
  return (ra3UNIT *)Uni;
} /* End of 'RA3_UnitCreateMario' function */

static VOID RA3_UnitRender( ra3UNIT_CONTROL *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitRender' function */




/* END OF 'U_CONTRL.C' FILE */

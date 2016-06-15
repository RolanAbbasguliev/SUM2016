/* FILE NAME: U_CUBE.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Ball unit sample.
 */

#include <time.h>

#include "anim.h"

/* Ball unit representation type */
typedef struct
{
  ra3UNIT;         /* Base unit fields */
  VEC Pos;         /* Cube position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  ra3PRIM Cow;     /* Cow primitive */
} ra3UNIT_CUBE;

static VOID RA3_UnitInit( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd1() * 5, Rnd1() * 5, Rnd1() * 5);
  Uni->TimerShift = Rnd1() * 30;
  Uni->TimerSpeed = Rnd1() * 8;
  RA3_RndPrimLoad(&Uni->Cow, "models\\havoc.g3d");
} /* End of 'RA3_UnitInit' function */

static VOID RA3_UnitClose( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
  RA3_RndPrimFree(&Uni->Cow);
} /* End of 'RA3_UnitClose' function */

static VOID RA3_UnitResponse( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_CUBE *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitRender( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
  RA3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.02, 0.02, 0.02)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                 VecMulNum(VecSet(Ani->JX + 2, Ani->JY + 2, Ani->JZ + 2), 3)))));
  RA3_RndPrimDraw(&Uni->Cow);
} /* End of 'RA3_UnitRender' function */

/* Unit cube creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ra3UNIT *) pointer to created unit.
 */
ra3UNIT * RA3_UnitCreateCube( VOID )
{
  ra3UNIT_CUBE *Uni;

  if ((Uni = (ra3UNIT_CUBE *)RA3_AnimUnitCreate(sizeof(ra3UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)RA3_UnitInit;
  Uni->Close = (VOID *)RA3_UnitClose;
  Uni->Response = (VOID *)RA3_UnitResponse;
  Uni->Render = (VOID *)RA3_UnitRender;
  return (ra3UNIT *)Uni;
} /* End of 'RA3_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */

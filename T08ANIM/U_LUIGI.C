  /* FILE NAME: U_LUIGI.C
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
  VEC Pos;         /* Luigi position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  ra3OBJ Luigi;     /* Luigi primitive */
} ra3UNIT_LUIGI;



static VOID RA3_UnitInit( ra3UNIT_LUIGI *Uni, ra3ANIM *Ani )
{
  Uni->Pos = VecSet( Rnd1() * 2, Rnd1() * 2, Rnd1() * 2);
  Uni->TimerShift = Rnd1() * 30;
  Uni->TimerSpeed = 0;
  RA3_RndObjLoad(&Uni->Luigi, "mario\\luigi.g3d");
} /* End of 'RA3_UnitInit' function */

static VOID RA3_UnitClose( ra3UNIT_LUIGI *Uni, ra3ANIM *Ani )
{
  RA3_RndObjFree(&Uni->Luigi);
} /* End of 'RA3_UnitClose' function */

static VOID RA3_UnitResponse( ra3UNIT_LUIGI *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_LUIGI *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitRender( ra3UNIT_LUIGI *Uni, ra3ANIM *Ani )
{
  RA3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(1, 1, 1)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                 VecMulNum(VecSet(Ani->JX *2, Ani->JY * 2, Ani->JZ * 2), 3)))));
  RA3_RndObjDraw(&Uni->Luigi);
} /* End of 'RA3_UnitRender' function */

/* Unit luigi creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ra3UNIT *) pointer to created unit.
 */
ra3UNIT * RA3_UnitCreateLuigi( VOID )
{
  ra3UNIT_LUIGI *Uni;

  if ((Uni = (ra3UNIT_LUIGI *)RA3_AnimUnitCreate(sizeof(ra3UNIT_LUIGI))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)RA3_UnitInit;
  Uni->Close = (VOID *)RA3_UnitClose;
  Uni->Response = (VOID *)RA3_UnitResponse;
  Uni->Render = (VOID *)RA3_UnitRender;
  return (ra3UNIT *)Uni;
} /* End of 'RA3_UnitCreateLuigi' function */

/* END OF 'U_LUIGI.C' FILE */

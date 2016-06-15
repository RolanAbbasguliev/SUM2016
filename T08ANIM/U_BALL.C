/* FILE NAME: U_BALL.C
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
  VEC Pos;         /* Ball position */
  VEC Shift;       /* Ball shift position */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  COLORREF Color;  /* Color */
} ra3UNIT_BALL;


static VOID RA3_UnitInit( ra3UNIT_BALL *Uni, ra3ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 1024, Rnd0() * 768, 0);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
  Uni->Color = RGB (0, 12, 12);
} /* End of 'RA3_UnitInit' function */

/* Unit ball deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_BALL *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitClose( ra3UNIT_BALL *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitClose' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_BALL *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitResponse( ra3UNIT_BALL *Uni, ra3ANIM *Ani )
{
  DBL t = Uni->TimerSpeed * Ani->GlobalTime + Uni->TimerShift;

  Uni->Shift = VecSet(30 * sin(t), 30 * cos(t), 0);
} /* End of 'RA3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_BALL *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitRender( ra3UNIT_BALL *Uni, ra3ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  p = p;
} /* End of 'RA3_UnitRender' function */

ra3UNIT * RA3_UnitCreateBall( VOID )
{
  ra3UNIT_BALL *Uni;

  if ((Uni = (ra3UNIT_BALL *)RA3_AnimUnitCreate(sizeof(ra3UNIT_BALL))) == NULL)
    return NULL;
 /* Setup unit methods */
  Uni->Init = (VOID *)RA3_UnitInit;
  Uni->Close = (VOID *)RA3_UnitClose;
  Uni->Response = (VOID *)RA3_UnitResponse;
  Uni->Render = (VOID *)RA3_UnitRender;
  return (ra3UNIT *)Uni;
} /* End of 'RA3_UnitCreateBall' function */

/* END OF 'U_BALL.C' FILE */
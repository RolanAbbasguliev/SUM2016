/* FILE NAME: U_BALL.C
 * PROGRAMMER: RA3
 * DATE: 14.06.2016
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
} ra3UNIT_CUBE;

/* Cube points */
VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

/* Cube edges */
INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

/* Cube primitive */
ra3PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_CUBE *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitInit( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd0() * 5, Rnd0() * 5, Rnd0() * 5);
  Uni->TimerShift = Rnd1() * 59;
  Uni->TimerSpeed = Rnd1() * 8;
} /* End of 'RA3_UnitInit' function */

/* Unit cube deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_CUBE *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitClose( ra3UNIT_CUBE *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitClose' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT_CUBE *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
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
  RA3_RndMatrWorld = MatrMulMatr(MatrScale(VecSet(0.1, 0.1, 0.1)),
                     MatrMulMatr(MatrRotateY((Uni->TimerSpeed * Ani->Time) * 30 + Uni->TimerShift),
                                 MatrTranslate(VecAddVec(Uni->Pos,
                                                         VecMulNum(VecSet(Ani->JX, Ani->JY, Ani->JZ), 3)))));
  RA3_RndPrimDraw(&Cube);
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


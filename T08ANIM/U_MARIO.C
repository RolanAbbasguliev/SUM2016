/* FILE NAME: U_CUBE.C
 * PROGRAMMER: RA3
 * DATE: 14.06.2016
 * PURPOSE: Ball unit sample.
 */

#include <time.h>

#include "anim.h"

/* Ball unit representation type */
typedef struct
{
  ra3UNIT;         /* Base unit fields */
  DBL TimerShift;  /* Timer shift phase value*/
  DBL TimerSpeed;  /* Timer speed value*/
  ra3OBJ Mario;      /* Mario primitive */
  ra3PRIM Pr;
  ra3PRIM Land;
  VEC Pos;         /* Model position */
  FLT DirAngle;    /* Model oriantation */
  FLT Speed;       /* Model movement speed */
  FLT Twist;       /* Model twist */
  VEC CamPos;      /* Camera position */
  UINT tx;
} ra3UNIT_CUBE;

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
  Uni->CamPos = VecSet(30, 15, 8);
  Uni->Pos = VecSet(0, 3, 0);
  Uni->DirAngle = 0;
  /*
  RA3_RndObjLoadMatrix = MatrMulMatr(MatrScale(VecSet1(0.01)), MatrRotateY(90));
  RA3_RndObjLoad(&Uni->Mario, "models\\mi.g3d");
  */
  RA3_RndObjLoadMatrix = MatrMulMatr(MatrMulMatr(MatrTranslate(VecSet(0, 1, -5)), MatrScale(VecSet1(0.1))), MatrRotateY(90));
  RA3_RndObjLoad(&Uni->Mario, "mario\\mario.g3d");

  /*
  ra3IMG Im;
  RA3_RndPrimCreateSphere(&Uni->Pr, 1, 30, 30);
  RA3_ImageLoad(&Im, "sky.bmp");
  Uni->Pr.MtlNo = RA3_RndMtlFromImage(&Im, "sky_sphere");
  RA3_ImageFree(&Im);
  RA3_RndPrimCreateLandscape(&Uni->Land, "GreenLand.BMP");
  RA3_ImageLoad(&Im, "GreenLand.BMP");
  */
}  /* End of 'RA3_UnitInit' function */
 

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
  RA3_RndObjFree(&Uni->Mario);
  RA3_RndPrimFree(&Uni->Pr);
  RA3_RndPrimFree(&Uni->Land);
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
  VEC V, Shift, POI;

  Uni->Speed +=
    (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] - Ani->JY * 1) * Ani->DeltaTime;
  Uni->DirAngle +=
    30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT] + Ani->Mdx * 1 - Ani->JX * 1) * Ani->DeltaTime;
  Uni->Twist = -30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT] + Ani->Mdx * 1 - Ani->JX * 1);

  V = VecMulMatr(VecSet(1, 0, 0), MatrRotateY(Uni->DirAngle));
  Shift = VecMulMatr(VecSet(-3, 3, 0), MatrRotateY(Uni->DirAngle));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(V, Uni->Speed * Ani->DeltaTime));

  POI = VecAddVec(Uni->Pos, Shift);
  Uni->CamPos =
    VecAddVec(Uni->CamPos,
      VecMulNum(VecSubVec(POI, Uni->CamPos), Ani->DeltaTime));
  RA3_RndMatrView = MatrView(Uni->CamPos, Uni->Pos, VecSet(0, 1, 0));
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
  RA3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrRotateX(Uni->Twist), MatrRotateY(Uni->DirAngle)), MatrTranslate(Uni->Pos));
  RA3_RndObjDraw(&Uni->Mario);

  RA3_RndMatrWorld = MatrScale(VecSet(30, 30, 30));
  RA3_RndPrimDraw(&Uni->Pr);
  RA3_RndMatrWorld = MatrScale(VecSet(27, 3, 27));
  RA3_RndPrimDraw(&Uni->Land);
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

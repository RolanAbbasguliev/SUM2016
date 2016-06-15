/* FILE NAME: UNITS.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Units base functions.
 */

#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitInit( ra3UNIT *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - anim3ation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitClose( ra3UNIT *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitResponse( ra3UNIT *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitRender( ra3UNIT *Uni, ra3ANIM *Ani )
{
} /* End of 'RA3_UnitRender' function */


ra3UNIT * RA3_AnimUnitCreate( INT Size )
{
  ra3UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ra3UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = RA3_UnitInit;
  Uni->Close = RA3_UnitClose;
  Uni->Response = RA3_UnitResponse;
  Uni->Render = RA3_UnitRender;
  return Uni;
} /* End of 'RA3_AnimUnitCreate' function */

/* END OF 'UNITS.C' FILE */

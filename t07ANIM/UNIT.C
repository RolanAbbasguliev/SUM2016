/*File Name: T07ANIM
 * Programmer: RA3
 * Date: 10.06.2016
 * PURPOSE: WinAPI windowed applictaion sample
 */

#include "anim.h"
#include "def.h"
#include "vec.h"
/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - animation context:
 *       RA3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitInit( ra3UNIT *Uni, ra3Anim *Ani )
{
} /* End of 'RA3_UnitInit' function */


/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       RA3UNIT *Uni;
 *   - animation context:
 *       RA3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitClose( ra3UNIT *Uni, ra3Anim *Ani )
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
static VOID RA3_UnitResponse( ra3UNIT *Uni, ra3Anim *Ani )
{
} /* End of 'ra3_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ra3UNIT *Uni;
 *   - animation context:
 *       ra3ANIM *Ani;
 * RETURNS: None.
 */
static VOID RA3_UnitRender( ra3UNIT *Uni, ra3Anim *Ani )
{
} /* End of 'ra3_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ra3UNIT *) pointer to created units */
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
} /* End of 'ra3_AnimUnitCreate' function */



/*File Name: T07ANIM
   * Programmer: RA3
 * Date: 10.06.2016
 */


#ifndef __ANIM_H_
#define __ANIM_H_

#pragma warning (disable: 4244)
#define RA3_MAX_UNITS 100

#include "def.h"

typedef struct tagra3UNIT ra3UNIT;
typedef struct 
{
  HWND hWnd;
  HDC hDC;  
  INT W, H;  
  HBITMAP hFrame;
  ra3UNIT *Units[RA3_MAX_UNITS];
  INT NumOfUnits;
  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL IsPause;
  /*Input system */
  INT Mx, M
} ra3Anim;
struct tagra3UNIT
{
  VOID (*Init)( ra3UNIT *Uni, ra3Anim *Ani );
  VOID (*Close)( ra3UNIT *Uni, ra3Anim *Ani );
  VOID (*Response)( ra3UNIT *Uni, ra3Anim *Ani );
  VOID (*Render)( ra3UNIT *Uni, ra3Anim *Ani );
};

extern ra3Anim ra3_Anim;

VOID ra3_AnimCopyFrame( HDC hDC );
VOID ra3_AnimInit( HWND hWnd );
VOID ra3_AnimAddUnit( ra3UNIT *Uni );
VOID ra3_AnimClose( VOID );
VOID ra3_Reasize( INT W, INT H );
VOID ra3_AnimRender( VOID );
VOID ra3_AnimDoExit( VOID );
ra3UNIT * ra3_AnimUnitCreate( INT Size );
#endif

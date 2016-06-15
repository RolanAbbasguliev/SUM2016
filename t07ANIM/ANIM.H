/*File Name: ANIM.H
 * Programmer: RA3
 * Date: 10.06.2016
 * PURPOSE: Animation system definitions. 
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

#pragma warning (disable: 4244)

/* Max unit number */
#define RA3_MAX_UNITS 100

typedef struct
{
  VEC *P;          /* Primitive points array */
  INT NumOfP;      /* Point array size */
  INT (*Edges)[2]; /* Edges array - array of point pairs */
  INT NumOfE;      /* Edges array size */
} ra3PRIM;

typedef struct tagra3UNIT ra3UNIT; 
typedef struct  // Animation context represtation type 
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
  INT Mx, My, Mz,
      Mdx, Mdy, Mdz;
  BYTE
    Keys[256],
    OldKeys[256],
    KeysClick[256];
  BYTE
    JBUT[32];
  INT JPov;
  DBL
    JX, JY, JZ, JR;
} ra3ANIM;

struct tagra3UNIT  // Animation UNIT represtation type 
{
  VOID (*Init)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Close)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Response)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Render)( ra3UNIT *Uni, ra3ANIM *Ani );
};

extern INT RA3_MouseWheel;

extern ra3ANIM RA3_Anim;

VOID RA3_AnimInit( HWND hWnd );
VOID RA3_AnimClose( VOID );
VOID RA3_AnimReasize( INT W, INT H );
VOID RA3_AnimCopyFrame( HDC hDC );
VOID RA3_AnimRender( VOID );
VOID RA3_AnimAddUnit( ra3UNIT *Uni );
VOID RA3_AnimDoExit( VOID );
VOID RA3_AnimFliFullScreen( VOID );
ra3UNIT * RA3_AnimUnitCreate( INT Size );

/* Global transformation matrices */
extern MATR
  RA3_RndMatrWorld, /* World (object) transformation matrix */
  RA3_RndMatrView,  /* Camera view transform */
  RA3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
extern DBL
  RA3_RndProjDist, /* Near clip plane */
  RA3_RndFarClip,  /* Far clip plane */
  RA3_RndProjSize; /* Project plane size */
#endif
/* End of 'ANIM.H' file */

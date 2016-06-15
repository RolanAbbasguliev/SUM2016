/* FILE NAME: ANIM.H
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
 * PURPOSE: Animation system definitions.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "image.h"

/* Maximum unit count */
#define RA3_MAX_UNITS 10000

/* Forward reference to unit type */
typedef struct tagra3UNIT ra3UNIT;

/* Animation context representation type */
typedef struct
{
  HWND hWnd;                     /* Frame window */
  HDC hDC;                       /* Drawing window context */
  HGLRC hGLRC;                   /* Rendering context */

  INT W, H;                      /* Frame size */
  ra3UNIT *Units[RA3_MAX_UNITS]; /* Animation units array */
  INT NumOfUnits;                /* Number of animation units */

  /* Timer data */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per seond value */
  BOOL IsPause;                  /* Pause flag */

  /* Input system */
  INT
    Mx, My,                      /* Mouse cursor position */
    Mz,                          /* Mouse wheel absolute value */
    Mdx, Mdy, Mdz;               /* Delta values of mouse axes */
  BYTE
    Keys[256],                   /* Current key states */
    OldKeys[256],                /* Previous frame key states */
    KeysClick[256];              /* Click key flags */
  BYTE
    JBut[32];                    /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    JX, JY, JZ, JR;              /* Joystick axes */
} ra3ANIM;

/* Animation unit representation type */
struct tagra3UNIT
{
  VOID (*Init)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Close)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Response)( ra3UNIT *Uni, ra3ANIM *Ani );
  VOID (*Render)( ra3UNIT *Uni, ra3ANIM *Ani );
};

/* Global mouse wheel delta value */
extern INT RA3_MouseWheel;

/* Global animation context */
extern ra3ANIM RA3_Anim;

VOID RA3_AnimInit( HWND hWnd );
VOID RA3_AnimClose( VOID );
VOID RA3_AnimResize( INT W, INT H );
VOID RA3_AnimCopyFrame( HDC hDC);
VOID RA3_AnimRender( VOID );
VOID RA3_AnimAddUnit( ra3UNIT *Uni );
VOID RA3_AnimDoExit( VOID );
VOID RA3_AnimFlipFullScreen( VOID );
ra3UNIT * RA3_AnimUnitCreate( INT Size );

/* Global transformation matrices */
extern MATR
  RA3_RndMatrWorld, /* World (object) transformation matrix */
  RA3_RndMatrView,  /* Camera view transform */
  RA3_RndMatrProj;  /* Projection transform */

/* Projection parameters */
extern FLT
  RA3_RndProjDist, /* Near clip plane */
  RA3_RndFarClip,  /* Far clip plane */
  RA3_RndProjSize; /* Project plane size */

/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;
  VEC4 C;  /* Normal at vertex */
} ra3VERTEX;

/* Primitive representation type */
typedef struct
{
  ra3VERTEX *V;     /* Primitive vertex array */
  INT NumOfV;       /* Point array size */
  INT *I;           /* Facets index array */
  INT NumOfI;       /* Facets index array size */
} ra3PRIM;

VOID RA3_RndSetProj( VOID );
VOID RA3_RndPrimDraw( ra3PRIM *Pr );
BOOL RA3_RndPrimLoad( ra3PRIM *Pr, CHAR *FileName );
VOID RA3_RndPrimFree( ra3PRIM *Pr );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */

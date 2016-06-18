/*
 * PROGRAMMER: RA3
 * DATE: 18.06.2016
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
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ra3UNIT *Uni;
   *   - animation context:
   *       ra3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( ra3UNIT *Uni, ra3ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ra3UNIT *Uni;
   *   - animation context:
   *       ra3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( ra3UNIT *Uni, ra3ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ra3UNIT *Uni;
   *   - animation context:
   *       ra3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( ra3UNIT *Uni, ra3ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       ra3UNIT *Uni;
   *   - animation context:
   *       ra3ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( ra3UNIT *Uni, ra3ANIM *Ani );
};

/***
 * Animation system support
 ***/

/* Global mouse wheel delta value */
extern INT RA3_MouseWheel;

/* Global animation context */
extern ra3ANIM RA3_Anim;

/* Animation system initialization function.
 * ARGUMENTS:
 *   - work window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID RA3_AnimInit( HWND hWnd );

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimClose( VOID );

/* Animation system resize window handle function.
 * ARGUMENTS:
 *   - new frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RA3_AnimResize( INT W, INT H );

/* Animation system resize window handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimCopyFrame( VOID );

/* Animation system render frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimRender( VOID );

/* Add new unit to animation system function.
 * ARGUMENTS:
 *   - pointer to new unit:
 *       ra3UNIT *Uni;
 * RETURNS: None.
 */
VOID RA3_AnimAddUnit( ra3UNIT *Uni );

/* End animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimDoExit( VOID );

/* Set/reset full screen mode function.
 * ARGUMENTS: None.
 * RETUNRS: None.
 */
VOID RA3_AnimFlipFullScreen( VOID );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ra3UNIT *) pointer to created unit.
 */
ra3UNIT * RA3_AnimUnitCreate( INT Size );

/***
 * Render support
 ***/

/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} ra3VERTEX;

/* Primitive representation type */
typedef struct
{
  /* OpenGL buffers */
  INT
    VA,       /* Vertex array */
    VBuf,     /* Vertex buffer */
    IBuf;     /* Index buffer */
  INT NumOfI; /* Facets index array size */
  MATR M;     /* Primitive transformation matrix */
  INT MtlNo;  /* Material number */
  INT Id;     /* Primitive Id */
} ra3PRIM;

/* Grid geometry representation type */
typedef struct
{
  INT W, H;     /* Grid size in vertices */
  ra3VERTEX *V; /* Grid points */
} ra3GRID;

/* Object representation type */
typedef struct
{
  ra3PRIM *Prims; /* Primitives array */
  INT NumOfPrims; /* Primitives array size */
  MATR M;         /* Object transformation matrix */
} ra3OBJ;

/* Material representation type */
typedef struct
{
  CHAR Name[300]; /* Material name */
  VEC Ka, Kd, Ks; /* Illumination coefficients */
  FLT Ph, Trans;  /* Shininess and Phong, transparency */
  INT TexW, TexH; /* Textiure image size */
  INT TexNo;      /* For load: bytes per pixel, in use: OpenGL texture no */
} ra3MTL;

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

/* Load object transform matrix */
extern MATR RA3_RndObjLoadMatrix;

/* Shader support */
extern UINT RA3_RndPrg;

/* Materials array */
#define RA3_MAX_MATERIALS 10000
extern ra3MTL RA3_RndMaterials[RA3_MAX_MATERIALS];
extern INT RA3_RndNumOfMaterials;

/* Setup projection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_RndSetProj( VOID );

/* Primitive create function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 *   - vertex array:
 *       ra3VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - INT array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID RA3_RndPrimCreate( ra3PRIM *Pr, ra3VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_RndPrimDraw( ra3PRIM *Pr );

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_RndPrimFree( ra3PRIM *Pr );

/* Object create function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjCreate( ra3OBJ *Obj );

/* Object free function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjFree( ra3OBJ *Obj );

/* Object draw function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 * RETURNS: None.
 */
VOID RA3_RndObjDraw( ra3OBJ *Obj );

/* Load object from '*.g3d' file function.
 * ARGUMENTS:
 *   - object structure pointer:
 *       ra3OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL RA3_RndObjLoad( ra3OBJ *Obj, CHAR *FileName );

/* Grid creation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 *   - grid size in vertices:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_GridCreate( ra3GRID *G, INT W, INT H );

/* Grid free function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 * RETURNS: None.
 */
VOID RA3_GridFree( ra3GRID *G );

/* Grid to primitive convert function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_Grid2Prim( ra3GRID *G, ra3PRIM *Pr );

/* Grid vertex normals evaluation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 * RETURNS: None.
 */
VOID RA3_GridEvalNormals( ra3GRID *G );

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_RndPrimCreateLandscape( ra3PRIM *Pr, CHAR *FileName );

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 *   - radius:
 *       FLT R;
 *   - split counts:
 *       INT N, M;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_RndPrimCreateSphere( ra3PRIM *Pr, FLT R, INT N, INT M );

/* Material find by name function.
 * ARGUMENTS:
 *   - material name:
 *       CHAR *Name;
 * RETURNS:
 *   (INT) number of found material or -1 if no result.
 */
INT RA3_RndFindMaterial( CHAR *Name );

/* Material load function.
 * ARGUMENTS:
 *   - material file name:
 *       CHAR *FileName;
 * RETURNS: None.
*/
VOID RA3_RndLoadMaterials( CHAR *FileName );

/* Make material from image function.
 * ARGUMENTS:
 *   - image pointer:
 *       ra3IMG *Im;
 *   - material name:
 *       CHAR *MtlName;
 * RETURNS:
 *   (INT) new material number.
 */
INT RA3_RndMtlFromImage( ra3IMG *Im, CHAR *MtlName );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader files prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (UINT) shader program index or 0 if error is occured.
 */
UINT RA3_RndShaderLoad( CHAR *FileNamePrefix );

/* Shader program load function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID RA3_RndShaderFree( UINT Prg );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */

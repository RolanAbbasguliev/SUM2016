 /*FILE NAME: ANIM.C
 * PROGRAMMER: RA3
 * DATE: 11.06.2016
 * PURPOSE: Animation system 
 */
#include <stdio.h>
#include "anim.h"
#include <mmsystem.h> 

#pragma comment(lib, "winmm")

#define RA3_GET_JOYSTIC_AXIR(A) \
  (2.0 *(ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) -1)

INT RA3_MouseWheel;
/* Global animation contex */
ra3ANIM RA3_Anim;
/* Time Local data */
static UINT64
  RA3_StartTime,
  RA3_OldTime,
  RA3_OldTimeFPS,
  RA3_PauseTime,
  RA3_TimePerSec,
  RA3_FrameCounter;

VOID RA3_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;
  memset(&RA3_Anim, 0, sizeof(ra3ANIM));
  /* Create window and create memory device context */
  RA3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  RA3_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  QueryPerformanceFrequency(&t);
  RA3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  RA3_StartTime = RA3_OldTime = RA3_OldTimeFPS = t.QuadPart;

  RA3_RndMatrWorld  = MatrIdentity();
  RA3_RndMatrView = MatrView(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
  RA3_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);

  RA3_RndProjDist = 1;
  RA3_RndFarClip = 500;
  RA3_RndProjSize = 1;

}/* End of 'RA3_AnimInit' function */

VOID RA3_AnimClose( VOID )
{
  int i;

  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
    RA3_Anim.Units[i]->Close(RA3_Anim.Units[i], &RA3_Anim);
    free(RA3_Anim.Units[i]);
  }
  RA3_Anim.NumOfUnits = 0;
  DeleteDC(RA3_Anim.hDC);
  DeleteObject(RA3_Anim.hFrame);
  memset(&RA3_AnimClose, 0, sizeof(ra3ANIM));
}/* End of 'RA3_AnimClose' function */

VOID RA3_AnimReasize( INT W, INT H )
{
  HDC hDC;
  
  RA3_Anim.W = W;
  RA3_Anim.H = H;

  /* create double buffer image */
  if (RA3_Anim.hFrame != NULL)
    DeleteObject(RA3_Anim.hFrame);
  hDC = GetDC(RA3_Anim.hWnd);
  RA3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(RA3_Anim.hWnd, hDC);
  SelectObject(RA3_Anim.hDC, RA3_Anim.hFrame);
}/* End of 'RA3_AnimReasize' function */

VOID RA3_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, RA3_Anim.W, RA3_Anim.H, RA3_Anim.hDC, 0, 0, SRCCOPY);
}/* End of 'RA3_AnimCopyFrame' function */

VOID RA3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;

  RA3_FrameCounter++;
  QueryPerformanceCounter(&t);

  RA3_Anim.GlobalTime = (DBL)(t.QuadPart - RA3_StartTime) / RA3_TimePerSec;
  RA3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - RA3_OldTime) / RA3_TimePerSec;
  
  if (RA3_Anim.IsPause)
  {
    RA3_Anim.DeltaTime = 0;
    RA3_PauseTime += t.QuadPart - RA3_OldTime;
  }
  else
  {
    RA3_Anim.DeltaTime = RA3_Anim.GlobalDeltaTime;
    RA3_Anim.Time = (DBL)(t.QuadPart - RA3_PauseTime - RA3_StartTime) / RA3_TimePerSec;
  }
  // FPS
  if (t.QuadPart - RA3_OldTimeFPS > RA3_TimePerSec)
  {
    CHAR str[100];

    RA3_Anim.FPS = RA3_FrameCounter * RA3_TimePerSec / (DBL)(t.QuadPart - RA3_OldTimeFPS);
    RA3_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS is: %.5f", str);
    RA3_FrameCounter = 0;
  }
  RA3_OldTime = t.QuadPart;
  /*** Obtain input system state ***/
  /* Keyboard */
  GetKeyboardState(RA3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    RA3_Anim.Keys[i] >>= 7;
    if (!RA3_Anim.OldKeys[i] && RA3_Anim.Keys[i])
      RA3_Anim.KeysClick[i] = TRUE;
    else
      RA3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(RA3_Anim.OldKeys, RA3_Anim.Keys, 256);

  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
    RA3_Anim.Units[i]->Response(RA3_Anim.Units[i], &RA3_Anim);
  /* clear background */
  SelectObject(RA3_Anim.hDC, GetStockObject(DC_PEN));
  SelectObject(RA3_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(RA3_Anim.hDC, RGB(255, 255, 255));
  Rectangle(RA3_Anim.hDC, 0, 0, RA3_Anim.W + 1, RA3_Anim.H + 1);
  /*** Render All Units ***/
  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
     SelectObject(RA3_Anim.hDC, GetStockObject(DC_PEN));
     SelectObject(RA3_Anim.hDC, GetStockObject(DC_BRUSH));
     SetDCPenColor(RA3_Anim.hDC, RGB(255, 255, 100));
     SetDCBrushColor(RA3_Anim.hDC, RGB(255, 127, 80));
     
     RA3_Anim.Units[i]->Render(RA3_Anim.Units[i], &RA3_Anim);
  }
}/* End of 'RA3_AnimRender' function */

VOID RA3_AnimAddUnit( ra3UNIT *Uni )
{
  if (RA3_Anim.NumOfUnits < RA3_MAX_UNITS)
  { 
    RA3_Anim.Units[RA3_Anim.NumOfUnits] = Uni;
    RA3_Anim.Units[RA3_Anim.NumOfUnits]->Init(RA3_Anim.Units[RA3_Anim.NumOfUnits], &RA3_Anim);
    RA3_Anim.NumOfUnits++;
  }
}/* End of 'RA3_AnimAddUnit' function */ 

VOID RA3_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;
  if (IsExit)
    return;
  IsExit = TRUE;
  /*DestroyWindow(RA3_Anim.hWnd)*/
  PostMessage(RA3_Anim.hWnd, WM_CLOSE, 0, 0);
}/* End of 'RA3_AnimDoExit' function */ 


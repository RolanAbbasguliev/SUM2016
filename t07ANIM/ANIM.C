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
ra3Anim RA3_Anim;
/* Time Local data */
static UINT64
  RA3_StartTime,
  RA3_OldTime,
  RA3_OldTimeFPS,
  RA3_PauseTime,
  RA3_TimerPerSec,
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
}












VOID RA3_Reasize( INT W, INT H )
{
  HDC hDC;
  
  RA3_Anim.W = W;
  RA3_Anim.H = H;

  if (RA3_Anim.hFrame != NULL)
    DeleteObject(RA3_Anim.hFrame);
  hDC = GetDC(RA3_Anim.hWnd);
  RA3_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(RA3_Anim.hWnd, hDC);
  SelectObject(RA3_Anim.hDC, RA3_Anim.hFrame);
}

VOID RA3_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, RA3_Anim.W, RA3_Anim.H, RA3_Anim.hDC, 0, 0, SRCCOPY);
}

VOID RA3_AnimAddUnit( ra3UNIT *Uni )
{
  if (ra3_Anim.NumOfUnits < MAX_UNITS)
  {  
    ra3_Anim.Units[ra3_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &ra3_Anim);
  }
}

VOID RA3_AnimDoExit( VOID )
{
  DestroyWindow(RA3_Anim.hWnd);
}


/* The start of 'PutLinetime' function */
VOID PutLineTime( HDC hDC, INT X, INT Y, INT X1, INT Y1 )
{
  MoveToEx(hDC, X, Y, NULL); 
  LineTo(hDC, X1, Y1);
}/* The end of 'PutLinetime' function */
VOID RA3_AnimRender( VOID )
{
  int i;

  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
    RA3_Anim.Units[i]->Response(RA3_Anim.Units[i], &RA3_Anim);

  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
    RA3_Anim.Units[i]->Render(RA3_Anim.Units[i], &RA3_Anim);
  }
}


 /*File Name: T07ANIM
 * Programmer: RA3
 * Date: 11.06.2016
 */

#include "anim.h"
#include "def.h"

ra3Anim RA3_Anim;

VOID RA3_AnimInit( HWND hWnd )
{
  HDC hDC;

  RA3_Anim.NumOfUnits = 0, memset(&RA3_Anim, 0, sizeof(ra3Anim));
  RA3_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  RA3_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
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

VOID RA3_AnimClose( VOID )
{
  int i;

  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
    RA3_Anim.Units[i]->Close(RA3_Anim.Units[i], &RA3_Anim);
    free(RA3_Anim.Units[i]);
  }
  DeleteDC(RA3_Anim.hDC);
  DeleteObject(RA3_Anim.hFrame);
  RA3_Anim.NumOfUnits = 0;
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


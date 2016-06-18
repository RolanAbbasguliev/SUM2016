/* FILE NAME: MAIN.C
 * PROGRAMMER: RA3
 * DATE: 15.06.2016
*/

#include "anim.h"
#include "units.h"

/* Window class name */
#define RA3_WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK RA3_MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
 /* INT i;  */
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();

  /* Create window class */
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL,IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = RA3_MyWindowFunc;
  wc.lpszClassName = RA3_WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONMASK);
    return 0;
  }
  /* Create window */
  hWnd =
    CreateWindow(RA3_WND_CLASS_NAME,
    "My First  Using OpenGL Project", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /*for (i = 0; i < 3000; i++)
    RA3_AnimAddUnit(RA3_UnitCreateBall());*/
  /* RA3_AnimAddUnit(RA3_UnitCreateClock()); */

  /* Run message loop */
 while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
} /* End of 'WinMain' function */

LRESULT CALLBACK RA3_MyWindowFunc( HWND hWnd, UINT Msg,
                                   WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    /* Setup timer */
    SetTimer(hWnd, 30, 2, NULL);
    RA3_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    RA3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    /* Send redraw timer message */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    RA3_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:
    /* Cancel draw background */
    return 0;
  case WM_TIMER:
    RA3_AnimRender();
    /* Post repaint window message */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    /* Redraw window frame */
    hDC = BeginPaint(hWnd, &ps);
    RA3_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    RA3_AnimClose();
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */


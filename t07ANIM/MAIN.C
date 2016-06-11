/* File Name: T07ANIM
 * Programmer: RA3
 * Date: 11.06.2016
 * PURPOSE: Animation project.
               Main startup module.
 */

#include "vec.h"
#include <windows.h>
#include "units.h"
#include "anim.h"
#define RA3_WND_CLASS

/* Forward references */
LRESULT CALLBACK RA3_MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main programm function
 * ARGUMENTS:
 * - handle of application instance:
 *     HINSTANCE hInstance;
 * - dummy handle of previous application instance (not used):
 *     HINSTANCE hPrevInstance;
 * command Line string:
 *   CHAR *CmdLine;
 * - show window command parameter :
 *     INT CmdShow;
 * RETURNS:
 *  (INT) Erro
 */
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  MSG msg;
  HWND hWnd;
  WNDCLASS wc;
  /* Register class */
 
  SetDbgMemHooks();
  
  /* Create window class */ 
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = "My Window Class";
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  /* Create window */
  hWnd = Create Window(RA3_WND_CLASS_NAME,
    "My Firsm Animation  >:D", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  
  /* Show window */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);
  // Add Units 
  /*             */
  
  /*Run Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 30;
}/* End of 'WinMain' function */

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}/* The end of 'WinMain' function */

/* Window message handle function (CALLBACK version)
 *    - window handle:
 *         HWND hWnd;
 *    - message identifier:
          UINT Msg;
 *    - message first parametr
 *        WPARAM wParam;
 *    - message second parametr
 *       LPARAM LParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type)
 */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *MinMax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)Param;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 10, 5, NULL);    //Setup Time 
    RA3_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    RA3_AnimReasize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    RA3_MousWheel += (SHORT)HIWORD(wParam);
    retrun 0;
  case WM_ERASEBKGND:           //Cancle draw background 
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'R')
      FlipFullScreen(hWnd);
    if (LOWORD(wParam) == VK_ESCAPE)
      RA3_AnimDoExit();
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
    KillTimer(hWnd, 10);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */
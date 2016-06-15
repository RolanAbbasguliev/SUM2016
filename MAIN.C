/* File Name: MAIN.C
 * Programmer: RA3
 * Date: 11.06.2016
 * PURPOSE: Animation project.
               Main startup module.
 */

#include <windows.h>
#include "units.h"
#include "anim.h"

#define RA3_WND_CLASS_NAME "My Window Class"

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
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = RA3_WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = RA3_MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(RA3_WND_CLASS_NAME, "MY first Animation System <[=-=]>", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  if (hWnd == NULL)
  {
    MessageBox(NULL, "Create window erroe", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  /* Show window */
  ShowWindow(hWnd, CmdShow);
  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} 
/* End of "WinMain" function */

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
LRESULT CALLBACK RA3_MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
    SetTimer(hWnd, 10, 5, NULL);    //Setup Time 
    RA3_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    RA3_AnimReasize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    RA3_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:           //Cancle draw background 
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
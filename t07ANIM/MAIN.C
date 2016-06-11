/* File Name: T07ANIM
 * Programmer: RA3
 * Date: 10.06.2016
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
 *RETURNS:
 *  (INT) Erro



*/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  MSG msg;
  HWND hWnd;
  WNDCLASS wc;

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

  RegisterClass(&wc);
  hWnd = CreateWindow("My Window Class", "First Animation Project", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 300, 500, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  
  return 30;
}/* The end of 'WinMain' function */


/* The start of 'MyWinFunc' function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  /*SetDbgMemHooks();*/

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 2, NULL);
    RA3_AnimInit(hWnd); 
  case WM_SIZE:
    RA3_Reasize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == 'R')
      FlipFullScreen(hWnd);
    if (LOWORD(wParam) == VK_ESCAPE)
      RA3_AnimDoExit();
    return 0;
  case WM_TIMER: 
    RA3_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
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

} /* End of 'MyWinFunc' function */
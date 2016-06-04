 /* File Name: T03POLE.C 
 * Programmer: RA3
 * DATE: 03.05.2016
 */
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#pragma warning(disable: 4244) 

#define WND_CLASS_NAME "My Window Class"

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
/* The main function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{                             
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME, "LocalTime", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  if (hWnd == NULL)
  {
    MessageBox(NULL, "Create window error", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  /* Show window */
  ShowWindow(hWnd, CmdShow);
  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 0;
}/* End of "WinMain" function */
       
/* Set/reset full screen mode function */
VOID FlipFullScreen( HWND hWnd )      
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
                                                      
  if (IsFullScreen)
  {
    /* Restore window size */                                     
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,                                  
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top  ,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;
                        
    /* Store window old size */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* Start of "Draw" fuction */
VOID Draw( HDC hDC, INT X, INT Y, POINT Pts )
{ 
  INT i, dx, dy;
  DOUBLE len, si, co;
  static POINT pt[] =
  {
    {-10, 50}, {-40, 100}, {300, 30} 
  };
  POINT pt1[sizeof (pt) / sizeof(pt[0])];

  dx = (Pts.x - X);
  dy = (Pts.y - Y);
  len = sqrt(dx * dx + dy * dy);
  si = dy / len;
  co = dx / len;

  for (i = 0; i < sizeof (pt) / sizeof(pt[0]); i++)
  {                                  
    pt1[i].x = X + pt[i].x * co - pt[i].y * si;
    pt1[i].y = Y + (pt[i].x * si + pt[i].y * co);
  }
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(rand(), rand(), rand()));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(rand(), rand(), rand()));
  Polygon(hDC, pt1, sizeof(pt1) / sizeof(pt1[0]));
}

/* Draw cursor trail image function */
VOID DrawTrail( HWND hWnd, HDC hDC )
{
  POINT p;
  INT i;

  GetCursorPos(&p);
  ScreenToClient(hWnd, &p);
  srand(30);
  for (i = 0; i < 300; i++)
    Draw(hDC, rand() % 2000, rand() % 1000, p);
}

/* Start of "MyWinFunc" function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME t;
  DOUBLE a, r;
  CHAR s[300];
  INT i;
  HFONT hFnt;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth,
      bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);

    DrawTrail(hWnd, hMemDC);

    InvalidateRect(hWnd, NULL, FALSE);
    return 30;
  case WM_PAINT:                                                             
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of "MyWinFunc" function */



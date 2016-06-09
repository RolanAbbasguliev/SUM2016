/* File Name: T01EYES.C 
 * Programmer: RA3
 * DATE: 02.05.2016
 */
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable: 4244) 

#define WND_CLASS_NAME "My Window Class"
#define N 30
#define M 15
#define PI 3.14159265358979

typedef double DBL;
typedef struct 
{
  DBL X, Y, Z;
} VEC;

VEC Rot( VEC V, DBL Angel)
{
  DBL A = Angel * PI / 180;
  VEC r;

  r.X = V.X;
  r.Y = V.Y * cos(A) - V.Z * sin(A);
  r.Z = V.Y * sin(A) + V.Z * cos(A);
  return r;
}
VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j, x, y;
  static VEC G[N][M]; 
  DBL theta, phi, phase = clock() / 10000.0;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255,127,80));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  for (i = 0; i < N; i++)
  {
    theta = i * PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * PI / (M - 1) + phase;

      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);

      G[i][j] = Rot(G[i][j], - 250 * sin(phase));
    }
  }
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
  }
  /* Horizontal lines */
  for (i= 0; i < N; i++)
   {
     x = Xc + G[i][0].X;
     y = Yc - G[i][0].Z;
     MoveToEx(hDC, x, y, NULL);
     for (j = 1; j < M; j++)
     {
       x = Xc + G[i][j].X;
       y = Yc - G[i][j].Z;
       LineTo(hDC, x, y);
     }
   }
  /* Vertical line*/ 
  for (j = 0; j < M; j++)
  {
    x = Xc + G[0][j].X;
    y = Yc - G[0][j].Z;
    MoveToEx(hDC, x, y, NULL);
    for (i = 1; i < N; i++)
    {
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      LineTo(hDC, x, y);
    }
  }
}

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
/* The main function */
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
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME, "30!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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

  return 0;
} 
/* End of "WinMain" function */

/* Start of "DrawEye" function */
VOID DrawEye(HWND hWnd, INT x, INT y, INT R, INT R1, HDC hDC)
{
  POINT pt;
  INT dx, dy;
  DOUBLE t;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  dx = pt.x - x;
  dy = pt.y - y;
  t = (R - R1) / sqrt( dx * dx + dy * dy);
  if (t < 1)
  {
    dx *= t;
    dy *= t;
  }
  
  Ellipse(hDC, x - R, y - R, x + R, y + R);
  Ellipse(hDC, x + dx - R1, y + dy - R1, x + dx + R1, y + dy + R1);
}
/* End of "DrawEye" function */

/* Start of "MyWinFunc" funcrion */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm;
  static HDC hMemDC;
 
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

 
    DrawEye(hWnd, 400, 400, 200, 50, hMemDC);
    DrawEye(hWnd, 800, 400, 200, 50, hMemDC);
    DrawSphere(hMemDC, w / 2, h / 2);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(120, 234, 0));
    TextOut(hMemDC, 30, 30, "Eye", 3);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
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
} 
/* End of "MyWinFunc" function */


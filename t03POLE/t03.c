 /* File Name: T03POLE.C 
 * Programmer: RA3
 * DATE: 03.05.2016
 */
#include <windows.h>
#include <math.h>
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
} 
/* End of "WinMain" function */
/* Start of "Draw" fuction */
VOID Draw(HDC hDC, INT X, INT Y, DOUBLE A)
{ 
  INT i;
  DOUBLE rad = A * 3.14159265358979 / 180, si = sin(rad), co = cos(rad);
  static POINT pt[]=
  {
    {-10, 0}, {10, 50}, {10, 50}, {10, 0}
  };
  static POINT pt1[sizeof (pt) / sizeof(pt[0])];

  for (i = 0; i < sizeof (pt) / sizeof(pt[0]); i++)
  {
    pt1[i].x = X + pt[i].x * co - pt[i].y * si;
    pt1[i].y = Y + pt[i].x * si + pt[i].y * co;
  }
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Polygon(hDC, pt1, sizeof(pt) / sizeof(pt[0]));
}
/* End of "Draw" function */


/* Start of "MyWinFunc" function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HPEN hPen, hOld;
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME t;
  DOUBLE a, r;
  CHAR s[300];
  HFONT hFnt;
  static INT w, h;
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "h.bmp", IMAGE_BITMAP, 550, 600, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);
    
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
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
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight)/ 2, bm.bmWidth,
      bm.bmHeight, hMemDCLogo, 0, 0, SRCCOPY);

    

    GetLocalTime(&t); 
    hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));                /* Color Of Hands */
    SelectObject(hMemDC, hPen);
    a = (t.wSecond * 2 * 3.14159265358979 / 60);
    r = (bm.bmWidth / 2.2);                                       /* Second: Hande */ 
    MoveToEx(hMemDC, w / 2, h /2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    DeleteObject(hPen);
    
    
    
    a = ((t.wMinute + t.wSecond / 60.0) * 2 * 3.14159265358979 / 60);
    r = (bm.bmWidth / 3.2);                                       /* Minute: Hande */
    MoveToEx(hMemDC, w / 2, h /2, NULL);                           
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);

    a = ((t.wHour % 12 + t.wMinute / 60.0) * 2 * 3.14159265358979 / 12);
    r = (bm.bmWidth / 4.2);                                       /* Hour: Hande */
    MoveToEx(hMemDC, w / 2, h /2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);

    SetBkMode(hMemDC, TRANSPARENT);  
    SetTextColor(hMemDC, RGB(123, 0, 125));
    TextOut(hMemDCLogo, 0, 550, s, sprintf(s, "0%i.0%i.%i", t.wDay, t.wMonth, t.wYear));

    SetTextColor(hMemDC, RGB(255, 0, 0));
    TextOut(hMemDC, 30, 30, "Current Time is on the picture", 5);
    Draw(hMemDC, 90, 90, 200);
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

/* End of file */


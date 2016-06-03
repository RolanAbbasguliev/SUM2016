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
  hWnd = CreateWindow(WND_CLASS_NAME, "LocalTime", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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

/* Start of "MyWinFunc" function */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  HPEN hPen, hOld;
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
    

    hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hMemDC, hPen);
    a = (t.wSecond * 2 * 3.14159265358979 / 60);
    r = (bm.bmWidth / 2.2);                                       /* Second: line */ 
    MoveToEx(hMemDC, w / 2, h /2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);
    
    
    a = ((t.wMinute + t.wSecond / 60.0) * 2 * 3.14159265358979 / 60);
    r = (bm.bmWidth / 3.2);                                       /* Minute: line */
    MoveToEx(hMemDC, w / 2, h /2, NULL);                           
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);

    a = ((t.wHour % 12 + t.wMinute / 60.0) * 2 * 3.14159265358979 / 12);
    r = (bm.bmWidth / 4.2);                                       /* Hour: line */
    MoveToEx(hMemDC, w / 2, h /2, NULL);
    LineTo(hMemDC, w / 2 + sin(a) * r, h / 2 - cos(a) * r);

    SetBkMode(hMemDC, TRANSPARENT);  
    SetTextColor(hMemDC, RGB(123, 0, 125));
    TextOut(hMemDCLogo, 0, 550, s, sprintf(s, "0%i.0%i.%i", t.wDay, t.wMonth, t.wYear));


    TextOut(hMemDC, 30, 30, "Clock", 5);

    hFnt = CreateFont( 10, 0, 0, 0, 0, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, "Bookman Old Style");
    SelectObject(hDC, hFnt);
    TextOut(hMemDC, 100, 50, "Clock", 5);
    DeleteObject(hFnt);


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


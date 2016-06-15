/* FILE NAME: ANIM.C
 * PROGRAMMER: RA3
 * DATE: 11.06.2016
 * PURPOSE: Animation system.
 */

#include <stdio.h>

#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

/* Joystic axis value obtain macro function */
#define RA3_GET_JOYSTICK_AXIS(A) (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

/* Global mouse wheel delta value */
INT RA3_MouseWheel;

/* Global animation context */
ra3ANIM RA3_Anim;

/* Timer local data */
static UINT64
  RA3_StartTime,    /* Start program time */
  RA3_OldTime,      /* Time from program start to previous frame */
  RA3_OldTimeFPS,   /* Old time FPS measurement */
  RA3_PauseTime,    /* Time during pause period */
  RA3_TimePerSec,   /* Timer resolution */
  RA3_FrameCounter; /* Frames counter */

/* Animation system initialization function.
 * ARGUMENTS:
 *   - work window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID RA3_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER t;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&RA3_Anim, 0, sizeof(ra3ANIM));

  /* Store window and create memory device context */
  RA3_Anim.hWnd = hWnd;
  RA3_Anim.hDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(RA3_Anim.hDC, &pfd);
  DescribePixelFormat(RA3_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(RA3_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  RA3_Anim.hGLRC = wglCreateContext(RA3_Anim.hDC);
  wglMakeCurrent(RA3_Anim.hDC, RA3_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(RA3_Anim.hGLRC);
    ReleaseDC(RA3_Anim.hWnd, RA3_Anim.hDC);
    exit(0);
  }

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  RA3_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  RA3_StartTime = RA3_OldTime = RA3_OldTimeFPS = t.QuadPart;
  RA3_PauseTime = 0;

  /* Render setup */
  RA3_RndMatrWorld = MatrIdentity();
  RA3_RndMatrView = MatrView(VecSet(15, 15, 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
  RA3_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);

  RA3_RndProjDist = 1;
  RA3_RndFarClip = 500;
  RA3_RndProjSize = 1;

  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT, GL_LINE); */
} /* End of 'RA3_AnimInit' function */

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimClose( VOID )
{
  INT i;

  /* Destroy all units */
  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
    RA3_Anim.Units[i]->Close(RA3_Anim.Units[i], &RA3_Anim);
    free(RA3_Anim.Units[i]);
  }
  RA3_Anim.NumOfUnits = 0;

  /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(RA3_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(RA3_Anim.hWnd, RA3_Anim.hDC);

  memset(&RA3_Anim, 0, sizeof(ra3ANIM));
} /* End of 'RA3_AnimClose' function */

/* Animation system resize window handle function.
 * ARGUMENTS:
 *   - new frame size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID RA3_AnimResize( INT W, INT H )
{
  /* Store new window frame size */
  RA3_Anim.W = W;
  RA3_Anim.H = H;

  glViewport(0, 0, W, H);

  RA3_RndSetProj();
} /* End of 'RA3_AnimClose' function */

/* Animation system resize window handle function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimCopyFrame( HDC hDC )
{
  SwapBuffers(RA3_Anim.hDC);
} /* End of 'RA3_AnimCopyFrame' function */

/* Animation system render frame function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimRender( VOID )
{
  INT i;
  POINT pt;
  LARGE_INTEGER t;

  /*** Handle timer ***/
  RA3_FrameCounter++;
  QueryPerformanceCounter(&t);
  /* Global time */
  RA3_Anim.GlobalTime = (DBL)(t.QuadPart - RA3_StartTime) / RA3_TimePerSec;
  RA3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - RA3_OldTime) / RA3_TimePerSec;
  /* Time with pause */
  if (RA3_Anim.IsPause)
  {
    RA3_Anim.DeltaTime = 0;
    RA3_PauseTime += t.QuadPart - RA3_OldTime;
  }
  else
  {
    RA3_Anim.DeltaTime = RA3_Anim.GlobalDeltaTime;
    RA3_Anim.Time = (DBL)(t.QuadPart - RA3_PauseTime - RA3_StartTime) / RA3_TimePerSec;
  }
  /* FPS */
  if (t.QuadPart - RA3_OldTimeFPS > RA3_TimePerSec)
  {
    CHAR str[100];

    RA3_Anim.FPS = RA3_FrameCounter * RA3_TimePerSec / (DBL)(t.QuadPart - RA3_OldTimeFPS);
    RA3_OldTimeFPS = t.QuadPart;
    sprintf(str, "FPS: %.5f", RA3_Anim.FPS);
    SetWindowText(RA3_Anim.hWnd, str);
    RA3_FrameCounter = 0;
  }
  RA3_OldTime = t.QuadPart;

  /*** Obtain input system state ***/

  /* Keyboard */
  GetKeyboardState(RA3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    RA3_Anim.Keys[i] >>= 7;
    if (!RA3_Anim.OldKeys[i] && RA3_Anim.Keys[i])
      RA3_Anim.KeysClick[i] = TRUE;
    else
      RA3_Anim.KeysClick[i] = FALSE;
  }
  memcpy(RA3_Anim.OldKeys, RA3_Anim.Keys, 256);

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(RA3_Anim.hWnd, &pt);
  RA3_Anim.Mdx = pt.x - RA3_Anim.Mx;
  RA3_Anim.Mdy = pt.y - RA3_Anim.My;
  RA3_Anim.Mx = pt.x;
  RA3_Anim.My = pt.y;
  RA3_Anim.Mz += RA3_MouseWheel;
  RA3_Anim.Mdz = RA3_MouseWheel;
  RA3_MouseWheel = 0;

  /* Joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          RA3_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        RA3_Anim.JX = RA3_GET_JOYSTICK_AXIS (X);
        RA3_Anim.JY = RA3_GET_JOYSTICK_AXIS (Y);
        RA3_Anim.JZ = RA3_GET_JOYSTICK_AXIS (Z);
        RA3_Anim.JR = RA3_GET_JOYSTICK_AXIS (R);

        /* Point of view */
        RA3_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  /*** Send response to all units ***/
  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
    RA3_Anim.Units[i]->Response(RA3_Anim.Units[i], &RA3_Anim);

  /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*** Render all units ***/
  for (i = 0; i < RA3_Anim.NumOfUnits; i++)
  {
    RA3_RndMatrWorld = MatrIdentity();
    RA3_Anim.Units[i]->Render(RA3_Anim.Units[i], &RA3_Anim);
  }

  /* Finalize OpenGL drawing */
  glFinish();
} /* End of 'RA3_AnimRender' function */

/* Add new unit to animation system function.
 * ARGUMENTS:
 *   - pointer to new unit:
 *       RA3UNIT *Uni;
 * RETURNS: None.
 */
VOID RA3_AnimAddUnit( ra3UNIT *Uni )
{
  if (RA3_Anim.NumOfUnits < RA3_MAX_UNITS)
  {
    RA3_Anim.Units[RA3_Anim.NumOfUnits] = Uni;
    RA3_Anim.Units[RA3_Anim.NumOfUnits]->Init(RA3_Anim.Units[RA3_Anim.NumOfUnits], &RA3_Anim);
    RA3_Anim.NumOfUnits++;
    /* other version:
    RA3_Anim.Units[RA3_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &RA3_Anim);
    */
  }
} /* End of 'RA3_AnimAddUnit' function */

/* End animation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID RA3_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;

  if (IsExit)
    return;
  IsExit = TRUE;
  /* DestroyWindow(RA3_Anim.hWnd); */
  PostMessage(RA3_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'RA3_AnimDoExit' function */

/* Set/reset full screen mode function.
 * ARGUMENTS: None.
 * RETUNRS: None.
 */
VOID RA3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(RA3_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(RA3_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(RA3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(RA3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(RA3_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'RA3_AnimFlipFullScreen' function */

/* END OF 'ANIM.C' FILE */


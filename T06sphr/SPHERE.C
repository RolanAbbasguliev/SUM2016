/* File Name: T06SPHR.C 
 * Programmer: RA3
 * DATE: 07.06.2016
 */
#include <windows.h>
#include <math.h>
#include "sphere.h"
#include <stdlib.h>

#pragma warning(disable: 4244)

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
  SetDCPenColor(hDC, RGB(rand(), rand(), rand()));
  SetDCBrushColor(hDC, RGB(12, 0, 0));

  for (i = 0; i < N; i++)
  {
    theta = i * PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2 * PI / (M - 1) + phase;

      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].Y = R * sin(theta) * sin(phi);
      G[i][j].Z = R * cos(theta);

      G[i][j] = Rot(G[i][j], - 200 * sin(phase));
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
  /* Vertical lines */ 
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
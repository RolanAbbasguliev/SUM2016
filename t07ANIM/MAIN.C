/* File Name: T07ANIM
 * Programmer: RA3
 * Date: 08.05.2016
 */

#include "Vec.h"
#include <windows.h>
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  MATR M;

  M = MatrIdentity();
  M = MatrTranslate(VecSet(5, 5, 5));
  M = MatrScale(VecSet(5, 5, 5));
  M = MatrRotateX(100);
  M = MatrRotateY(100);
  M = MatrRotateZ(100);
  M = MatrRotate(30, VecSet(1, 2, 3));
  M = MatrTranspose(M);
  M = MatrInverse(m);
} 



} /* End of 'MinMain' function *
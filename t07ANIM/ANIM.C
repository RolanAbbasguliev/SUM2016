/*File Name: T07ANIM
 * Programmer: RA3
 * Date: 10.05.2016
 */

#include "vec.h"
#include "def.h"

ANIM.C - VG4_Anim***
  VG4_AnimInit(hWnd){
}

VG4_AnimClose(){
}

VG4_AnimResize(W, H){
}

VG4_AnimCopyFrame(HDC){
}

VG4_AnimRender(){
}

VG4_AnimAddUnit(Uni){
}
 VG4_Anim  //Global patametr - structur of animation contex 
vg4ANIM:
typedef struct
{
  HWND hWnd; //window 
  HDC hDC;   //contex in memory
  INT W, H;  //size of window 
  HBITMAP hFrame; //picture of frame- картинка кадра
  vg4UNIT *Units[VG4_MAX_UNITS]; //massive of animation abstacts - массив объектов анимации
  INT NumOfUnits; - //numbers of animation текущее количество объектов анимации
} vg4ANIM;





 /*File Name: T07ANIM
 * Programmer: RA3
 * Date: 10.06.2016
 */

typedef struct tagvg4UNIT vg4UNIT;
typedef struct
{
  . . . vg4UNIT * ...
} vg4ANIM;
struct tagvg4UNIT
{
  . . . vg4ANIM . . .
};


... VG4_Anim ...
... VG4_Unit ...




VOID VG4_AnimClose( VOID )
{
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
  }
  VG4_Anim.NumOfUnits = 0;
  . . .
}

VOID VG4_AnimRender( VOID )
{
  /  (kbd, mouse, joystick)
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  //Erase Frame
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    . . .// можно сбросить все кисти и перья
    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
  . . .
}

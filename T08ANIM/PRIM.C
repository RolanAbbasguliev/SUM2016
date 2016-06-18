/* FILE NAME: PRIM.C
 * PROGRAMMER: RA3
 * DATE: 18.06.2016
 * PURPOSE: Object handle functions.
 */

#include "anim.h"

/* Primitive create function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 *   - vertex array:
 *       ra3VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - INT array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID RA3_RndPrimCreate( ra3PRIM *Pr, ra3VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(ra3PRIM));

  Pr->M = MatrIdentity();
  Pr->NumOfI = NumOfI;

  /* Create OpenGL buffers */
  glGenVertexArrays(1, &Pr->VA);
  glGenBuffers(1, &Pr->VBuf);
  glGenBuffers(1, &Pr->IBuf);

  /* Activate vertex array */
  glBindVertexArray(Pr->VA);
  /* Activate vertex buffer */
  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  /* Store vertex data */
  glBufferData(GL_ARRAY_BUFFER, sizeof(ra3VERTEX) * NumOfV, V, GL_STATIC_DRAW);

  /* Setup data order */
  /*                    layout,
   *                      components count,
   *                          type
   *                                    should be normalize,
   *                                           vertex structure size in bytes (stride),
   *                                               offset in bytes to field start */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ra3VERTEX),
                        (VOID *)0); /* position */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ra3VERTEX),
                        (VOID *)sizeof(VEC)); /* texture coordinates */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ra3VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ra3VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

  /* Enable used attributes */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* Indices */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

  /* Disable vertex array */
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
} /* End of 'RA3_RndPrimCreate' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_RndPrimDraw( ra3PRIM *Pr )
{
  INT loc, mtl;
  MATR M, MSave;

  /* Build transform matrix */
  MSave = RA3_RndMatrWorld;
  RA3_RndMatrWorld = MatrMulMatr(RA3_RndMatrWorld, Pr->M);
  M = MatrMulMatr(RA3_RndMatrWorld, MatrMulMatr(RA3_RndMatrView, RA3_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Enable base shader */
  glUseProgram(RA3_RndPrg);

  mtl = Pr->MtlNo;
  if (mtl == -1)
    mtl = 0;
  if (RA3_RndMaterials[mtl].TexNo != 0)
  {
    glBindTexture(GL_TEXTURE_2D, RA3_RndMaterials[mtl].TexNo);
    if ((loc = glGetUniformLocation(RA3_RndPrg, "IsTexture")) != -1)
      glUniform1i(loc, 1);
  }
  else
    if ((loc = glGetUniformLocation(RA3_RndPrg, "IsTexture")) != -1)
      glUniform1i(loc, 0);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Ka")) != -1)
    glUniform3fv(loc, 1, &RA3_RndMaterials[mtl].Ka.X);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Kd")) != -1)
    glUniform3fv(loc, 1, &RA3_RndMaterials[mtl].Kd.X);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Ks")) != -1)
    glUniform3fv(loc, 1, &RA3_RndMaterials[mtl].Ks.X);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Ph")) != -1)
    glUniform1f(loc, RA3_RndMaterials[mtl].Ph);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Trans")) != -1)
    glUniform1f(loc, RA3_RndMaterials[mtl].Trans);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Id")) != -1)
    glUniform1i(loc, Pr->Id);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(RA3_RndPrg, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, RA3_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, RA3_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, RA3_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(RA3_RndPrg, "Time")) != -1)
    glUniform1f(loc, RA3_Anim.Time);

  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);;
  glUseProgram(0);
  RA3_RndMatrWorld = MSave;
} /* End of 'RA3_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_RndPrimFree( ra3PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(ra3PRIM));
} /* End of 'RA3_RndPrimFree' function */

/* END OF 'PRIM.C' FILE */

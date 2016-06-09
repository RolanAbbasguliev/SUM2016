/*File Name: T07ANIM
 * Programmer: RA3
 * Date: 08.05.2016
 */

#ifndef  __VEC_H_
#define  __VEC_H_
#include <math.h>
#define PI 3.14159265358979
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* base real type */
typedef double DBL;

/* type for vectors in space */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* type for matrix - massive in structer */



typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

/* Vectors */


/* Start of 'VecSet' function */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{ 
  VEC V;
  
  V.X = X;
  V.Y = Y;
  V.Z = Z;
  return V;
}/* End of 'VecSet' function */



/* Start of 'VecAddVec' function */
__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}/* End of 'VecAddVec' function */

/* Start of 'VecSubVec' function */
__inline VEC VecSubVec( VEC A, VEC B );
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
} /* End of 'VecSubVec' function */

/* Start of 'VecMulNum' function */
__inline VEC VecMulNum( VEC A, DBL N );
{
   return VecSet(A.X * N,A.Y * N, A.Z * N);
}              /* End of 'VecMulNum' function */
/* Start of 'VecDivNum' function */
__inline VEC VecDivNum( VEC A, DBL N );
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
}/* End of 'VecDivNum' function */

                                                                  /*Example for use:
                                                                  VEC S, V;
                                                                  DBL dt;

                                                                  math: S = S + V * dt;
                                                                  C:    S = VecAddVec(S, VecMulNum(V, dt));
                                                                  */


/* Rotate vector function */

__inline VEC Rotate( VEC P, VEC A, DBL Angle )
{
  DBL si, co;

  A = VecNormalize(A);

  Angle *= PI / 180;
  si = sin(Angle);
  co = cos(Angle);                                               
                                                                 
  return VecSet(P.X * (co + A.X * A.X * (1 - co)) +              
                P.Y * (A.Y * A.X * (1 - co) + A.Z * si) +        
                P.Z * (A.Z * A.X * (1 - co) - A.Y * si),         
                P.X * (A.X * A.Y * (1 - co) - A.Z * si) +        
                P.Y * (co + A.Y * A.Y * (1 - co)) +              
                P.Z * (A.Z * A.Y * (1 - co) + A.X * si),
                P.X * (A.X * A.Z * (1 - co) + A.Y * si) +
                P.Y * (A.Y * A.Z * (1 - co) - A.X * si) + 
                P.Z * (co + A.Z * A.Z * (1 - co)));
} /* End of 'Rotate' function */


/* Start of 'VecNeg' function */
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}/* End of 'VecNeg' function */


/* Start of 'VecCrossVec' function */         /* Vectoring composition of vectors */ 
__inline VEC VecCrossVec( VEC A, VEC B );
{
   return VecSet(A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X);
}/* End of 'VecCrossVec' function */

/* Start of 'VecLen2' function */       /* Vectots in ^2 */
__inline DBL VecLen2( VEC V );
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}/* End of 'VecLen2' function 

/* Start of 'VecLen' function */
__inline DBL VecLen( VEC V );
{
  DBL len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;

  if (len != 0 && len != 1)
    return sqrt(len);
  return len;
}/*End of 'VecLen' function */

/* Start of 'VecNormalize' function */
__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len != 1 && len != 0)
    len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
}/* End of 'VecNormalize' function */ 


/* Start of 'VecDotVec' function */
__inline VEC VecDotVec( VEC A, VEC B )
{
  return (A.X * B.X + A.Y * B.Y + A.Z * B.Z);
}/* End of 'VecDotVec' function */

/* Start of 'PointTransform' function */
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}

/* Start of 'VectorTransform' function */
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}/* End of 'VectorTransform' function */

/* Start of 'PointTransform4' function */
__inline VEC PointTransform4( VEC V, MATR M )
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}/* End of 'PointTransform4' function  */

/* Start of 'MatrTranslate' function  */

/* Start of 'UnitMatrix ' function */
static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};/* End of 'UnitMatrix ' function */

/* Start of 'MatrTranslate' function */
__inline MATR MatrTranslate( VEC D )
{
  MATR M =
  {
    {
      {  1,   0,   0,  0},
      {  0,   1,   0,  0},
      {  0,   0,   1,  0},
      {D.X, D.Y, D.Z,  1},
    }
  };

  return M;
}/* End of 'MatrTranslate' function */

/* Start of 'MatrScale' function */
__inline MATR MatrScale( VEC S )
{
  MATR M =
  {
    {
      {S.X,    0,    0,  0},
      {   0, S.Y,    0,  0},
      {   0,    0, S.Z,  0},
      {   0,    0,    0,  1}
    }
  };

  return M;
} /* End of 'MatrScale' function */

/* Start of 'MatrRotateX' function */
__inline MATR MatrRotateX( DBL AngleDegree );
{
  DBL A = D2R( AngleDegree ), si= sin(A), co = cos(A);
  MART M =
  {
    {
      {1,    0,  0,   0},
      {0,   co,  si,  0},
      {0,  -si,  co,  0},
      {0,    0,   0,  1}
    }
  };
  return M;
}


}/* End of 'MatrRotateX' function */

/* Start of 'MatrRotateY' function */
__inline MATR MatrRotateY( DBL AngleDegree );
{
  {
  DBL A = D2R( AngleDegree ), si= sin(A), co = cos(A);
  MART M =
  {
    {
      {co,  0,  -si,   0},
      {0,   1,      0, 0},
      {si,     0, co,   0},
      {0,    0,   0,    1}
    }
  };
  return M;
}/* End of 'MatrRotateY' function */

/* Start of 'MatrRotateZ' function */
__inline MATR MatrRotateZ( DBL AngleDegree );
{
  {
  DBL A = D2R( AngleDegree ), si= sin(A), co = cos(A);
  MART M =
  {
    {
      {co,   si,  0,   0},
      {-si,  co,  0,  0},
      {0,     0,  1,  0},
      {0,    0,   0,  1}
    }
  };
  return M;
}


}/* End of 'MatrRotateZ' function */

/* Start of 'MatrRotate' function */
__inline MATR MatrRotate( DBL AngleDegree, VEC R )            /* Marix of  turn */
{
  DBL A = D2R(AngleToDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),        V.X * V.Y * (1 - co) - V.Z * si,  V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,  co + V.Y * V.Y * (1 - co),        V.Y * V.Z * (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,  V.Z * V.Y * (1 - co) + V.X * si,  co + V.Z * V.Z * (1 - co),       0},
      {0,                                0,                                0,                               1}
    }
  };

  return M;
}/* End of 'MatrRotate' function */


/* Start of 'MatrIdentity' function */
__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}/* End of 'MatrIdentity' function */


/* Start of 'MatrMulMatr' function */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0, r.A[i][j] = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}/* End of 'MatrMulMatr' function */


/* Start of 'MatrTranspose' function */
__inline MATR MatrTranspose( MATR M );
{
  int i, j;
  MART r;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[i][j];
  return r;
}/* End of 'MatrTranspose' function */


/* Start of 'MatrDeterm3x3' function */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}/* End of 'MatrDeterm3x3' function */


/* Start of 'MatrDeterm' function */
__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}/* End of 'MatrDeterm' function */

/* Start of 'MatrInverse' function */
__inline MATR MatrInverse( MATR M )
{
  DBL det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return M;
  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                             M.A[2][1], M.A[2][2], M.A[2][3],
                             M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                             M.A[2][0], M.A[2][1], M.A[2][3],
                             M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                             M.A[1][0], M.A[1][2], M.A[1][3],
                             M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][2] = -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                             M.A[1][0], M.A[1][1], M.A[1][2],
                             M.A[2][0], M.A[2][1], M.A[2][2]) / det;

}/* End of 'MatrInverse' function */

/* Alternative */
/* Start of 'VecSubVec' function */
__inline MATR MatrInverse( MATR M )
{
  INT p[3][] =
  {
    {1, 2, 3}
    {0, 2, 3}
    {0, 1, 3}
    {0, 1, 2}
  };

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[j][i] =
        (1 - (i + j) % 2 * 2) * 
        MatrDeterm3x3(
          M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
          M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
          M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;
}/* End of 'MatrInverse' function */
#endif
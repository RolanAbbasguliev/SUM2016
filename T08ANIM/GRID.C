
/* FILE NAME: GRID.C
 * PROGRAMMER: RA3
 * DATE: 18.06.2016
 * PURPOSE: Grid geometry handle functions.
 */

#include "anim.h"

/* Grid creation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 *   - grid size in vertices:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_GridCreate( ra3GRID *G, INT W, INT H )
{
  INT i, x, y, size = (W - 1) * (H - 1) * 2 * 3 * sizeof(ra3VERTEX);

  memset(G, 0, sizeof(ra3GRID));
  if ((G->V = malloc(size)) == NULL)
    return FALSE;
  memset(G->V, 0, size);

  for (i = 0, y = 0; y < H; y++)
    for (x = 0; x < W; x++, i++)
    {
      G->V[i].P = VecSet(x / (W - 1.0) * 2 - 1, 0, 1 - y / (H - 1.0) * 2);
      G->V[i].N = VecSet(0, 1, 0);
      G->V[i].T = Vec2Set(x / (W - 1.0), y / (H - 1.0));
      G->V[i].C = Vec4Set(1, 1, 1, 1);
    }
  G->W = W;
  G->H = H;

  return TRUE;
} /* End of 'RA3_GridCreate' function */

/* Grid free function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 * RETURNS: None.
 */
VOID RA3_GridFree( ra3GRID *G )
{
  if (G->V != NULL)
    free(G->V);
  memset(G, 0, sizeof(ra3GRID));
} /* End of 'RA3_GridFree' function */

/* Grid to primitive convert function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 * RETURNS: None.
 */
VOID RA3_Grid2Prim( ra3GRID *G, ra3PRIM *Pr )
{
  INT
    p, x, y,
    n = (G->W - 1) * (G->H - 1) * 2 * 3,
    size = n * sizeof(ra3VERTEX);
  INT *I = malloc(size);

  memset(Pr, 0, sizeof(ra3PRIM));
  if (I == NULL)
    return;
  memset(I, 0, size);

  /* Build index array */
  for (p = 0, y = 0; y < G->H - 1; y++)
    for (x = 0; x < G->W - 1; x++)
    {
      /* 0 triangle */
      I[p++] = y * G->W + x;             /* (x,y)-> y * G->W + x */
      I[p++] = (y + 1) * G->W + (x + 1); /* (x+1,y+1)-> (y + 1) * G->W + (x + 1) */
      I[p++] = (y + 1) * G->W + x;       /* (x,y+1)-> (y + 1) * G->W + x */

      /* 1 triangle */
      I[p++] = y * G->W + x;             /* (x,y)-> y * G->W + x */
      I[p++] = y * G->W + (x + 1);       /* (x+1,y)-> y * G->W + (x + 1) */
      I[p++] = (y + 1) * G->W + (x + 1); /* (x+1,y+1)-> (y + 1) * G->W + (x + 1) */
    }

  RA3_RndPrimCreate(Pr, G->V, G->W * G->H, I, n);
} /* End of 'RA3_Grid2Prim' function */

/* Helpful triangle normal evaluation function.
 * ARGUMENTS:
 *   - triangle points:
 *       VEC P0, P1, P2;
 * RETURNS:
 *   (VEC) normal vector value.
 */
static VEC RA3_TriangleNormal( VEC P0, VEC P1, VEC P2 )
{
  return VecNormalize(VecCrossVec(VecSubVec(P1, P0), VecSubVec(P2, P0)));
} /* End of 'RA3_TriangleNormal' function */

/* Grid vertex normals evaluation function.
 * ARGUMENTS:
 *   - grid pointer:
 *       ra3GRID *G;
 * RETURNS: None.
 */
VOID RA3_GridEvalNormals( ra3GRID *G )
{
  INT
    p, x, y,
    n0, n1, n2;
  VEC N;

  /* Set to zero all vertex normqal vectors */
  for (p = 0; p < G->W * G->H; p++)
    G->V[p].N = VecSet(0, 0, 0);

  /* Evaluate all triangles normals */
  for (p = 0, y = 0; y < G->H - 1; y++, p++)
    for (x = 0; x < G->W - 1; x++, p++)
    {
      /* 0 triangle */
      n0 = p;
      n1 = p + G->W + 1;
      n2 = p + G->W;
      N = RA3_TriangleNormal(G->V[n0].P, G->V[n1].P, G->V[n2].P);
      G->V[n0].N = VecAddVec(G->V[n0].N, N);
      G->V[n1].N = VecAddVec(G->V[n1].N, N);
      G->V[n2].N = VecAddVec(G->V[n2].N, N);

      /* 1 triangle */
      n0 = p;
      n1 = p + 1;
      n2 = p + G->W + 1;
      N = RA3_TriangleNormal(G->V[n0].P, G->V[n1].P, G->V[n2].P);
      G->V[n0].N = VecAddVec(G->V[n0].N, N);
      G->V[n1].N = VecAddVec(G->V[n1].N, N);
      G->V[n2].N = VecAddVec(G->V[n2].N, N);
    }

  /* Normalize all vertex normqal vectors */
  for (p = 0; p < G->W * G->H; p++)
    G->V[p].N = VecNormalize(G->V[p].N);
} /* End of 'RA3_GridEvalNormals' function */

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_RndPrimCreateLandscape( ra3PRIM *Pr, CHAR *FileName )
{
  INT p, x, y;
  ra3IMG Im;
  ra3GRID G;

  memset(Pr, 0, sizeof(ra3PRIM));
  if (!RA3_ImageLoad(&Im, FileName))
    return FALSE;
  if (!RA3_GridCreate(&G, Im.W, Im.H))
  {
    RA3_ImageFree(&Im);
    return FALSE;
  }

  /* Setup heights */
  for (p = 0, y = 0; y < G.H; y++)
    for (x = 0; x < G.W; x++, p++)
    {
      DWORD c = RA3_ImageGetP(&Im, x, y);

      G.V[p].P.Y = ((c >> 8) & 0xFF) / 255.0;
    }
  RA3_GridEvalNormals(&G);
  RA3_Grid2Prim(&G, Pr);
  return TRUE;
} /* End of 'RA3_RndPrimCreateLandscape' function */

/* Create height field primitive from image function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ra3PRIM *Pr;
 *   - radius:
 *       FLT R;
 *   - split counts:
 *       INT N, M;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_RndPrimCreateSphere( ra3PRIM *Pr, FLT R, INT N, INT M )
{
  INT p, x, y;
  ra3GRID G;

  memset(Pr, 0, sizeof(ra3PRIM));
  if (!RA3_GridCreate(&G, M, N))
    return FALSE;

  /* Setup points */
  for (p = 0, y = 0; y < G.H; y++)
  {
    FLT theta = (G.H - 1 - y) * PI / (G.H - 1.0);
    for (x = 0; x < G.W; x++, p++)
    {
      FLT
        phi = x * 2 * PI / (G.W - 1.0),
        px = sin(theta) * sin(phi),
        py = cos(theta),
        pz = sin(theta) * cos(phi);

      G.V[p].P = VecSet(R * px, R * py, R * pz);
      G.V[p].N = VecSet(px, py, pz);
    }
  }
  RA3_Grid2Prim(&G, Pr);
  return TRUE;
} /* End of 'RA3_RndPrimCreateSphere' function */

/* END OF 'GRID.C' FILE */

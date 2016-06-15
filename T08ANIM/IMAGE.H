/* FILE NAME: IMAGE.H
 * PROGRAMMER: RA3
 * DATE: 13.06.2016
 * PURPOSE: Image handle decrlaration.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image represetation type */
typedef struct
{
  HBITMAP hImage; /* Image bitmap handle */
  INT W, H;       /* Image size */
  DWORD *Bits;    /* Image pixel color data */
} ra3IMG;

/* Load image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       RA3IMG *Im;
 *   - image file name (*.BMP):
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL RA3_ImageLoad( ra3IMG *Im, CHAR *FileName );

/* Free image function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       RA3IMG *Im;
 * RETURNS: None.
 */
VOID RA3_ImageFree( ra3IMG *Im );

/* Get image pixel color function.
 * ARGUMENTS:
 *   - image store data pointer:
 *       RA3IMG *Im;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) pixel color.
 */
DWORD RA3_ImageGetP( ra3IMG *Im, INT X, INT Y );


#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */

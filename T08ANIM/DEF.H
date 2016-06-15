/*File Name: DEF.H
 * Programmer: RA3
 * Date: 11.05.2016
 * PURPOSE: Base definitions.
 */

#ifndef __DEF_H_ 
#define __DEF_H_

#include <windows.h>

/* Debug memory allocation support */
#ifdef _DEBUG

#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
# define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */
#include <stdlib.h>

/* OpenGL specific includes */
#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "vec.h"

typedef FLOAT FLT;
typedef DOUBLE DBL;

#endif /* __DEF_H_ */

/* End of __DEF_H_ File */
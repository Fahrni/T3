// #define DEBUGGING

#include "windows.h"					// all windows funcs.
// #include "d:\qcwin\include\string.h"	// for strcpy
#include "string.h"	// for strcpy
#include "utl.h"

/****************************************************************************
*
* vDrawConcaveFrame
*
*		Draws a concave 3D looking box.
*
*****************************************************************************/

void nDrawConcaveFrame( HDC hDestDC, RECT rc )
{
#ifdef DEBUGGING
// the below two statements are for debugging on my mono monitor.
	static LOGPEN lpLtGray = { PS_SOLID, 1, 1, clrBLACK },
			  	  lpWhite  = { PS_SOLID, 1, 1, clrBLACK };
#else
	static LOGPEN lpLtGray = { PS_SOLID, 1, 1, clrDKGRAY },
			  	  lpWhite  = { PS_SOLID, 1, 1, clrWHITE };
#endif

HPEN hLtGray, hWhite;

	hLtGray = CreatePenIndirect( &lpLtGray );
	hWhite  = CreatePenIndirect( &lpWhite );

	MoveTo( hDestDC, rc.right, rc.top );

	// Paint the top and left side lines, light gray.
	SelectObject( hDestDC, hLtGray );
	LineTo( hDestDC, rc.left, rc.top );
	LineTo( hDestDC, rc.left, rc.bottom );

	// Paint the bottom and right side lines, white.
	SelectObject( hDestDC, hWhite );
	LineTo( hDestDC, rc.right, rc.bottom );
    LineTo( hDestDC, rc.right, rc.top );

    DeleteObject( hLtGray );
    DeleteObject( hWhite );

	// Rectangle( hDestDC, rc.left, rc.top, rc.right, rc.bottom );

}


/****************************************************************************
*
* vPadString
*
*		Pads the string "szStringToPad" with "nStrLength" spaces.
*
*****************************************************************************/

void vPadString( char *szStringToPad, int nStrLength )
{
	int nLoopCntr;

	for( nLoopCntr = strlen( szStringToPad ); nLoopCntr < ( nStrLength - 1 ); nLoopCntr++ )
		strcat( szStringToPad, " " );

}


/****************************************************************************
*
* BlitIt
*
* 	Bit Blit the bitmap specified in the hBitmap parameter to the
* 	device specified by the hDC parameter.
*
*	** NOTE ** : 	This function always places the bitmap at 0,0 and
*					always blits the entire bitmap from 0,0 to the
*					width and height of the bitmap.
*
*****************************************************************************/
void BlitIt( HDC hDC, HBITMAP hBitmap )
{
BITMAP bm;
HDC hMemDC;

	hMemDC = CreateCompatibleDC( hDC );

	SelectObject( hMemDC, hBitmap );

	GetObject( hBitmap, sizeof(BITMAP), (LPSTR) &bm );

	BitBlt( hDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY );

	DeleteDC( hMemDC );
}
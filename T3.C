/****************************************************************************

    T3.c - Tic-Tac-Toe for Windows.

****************************************************************************/
// #define DEBUGGING                      // Turns on debugging.

#include "windows.h"                    // required for all Windows applications
#include "stdlib.h"                     // for itoa.
#include "string.h"                     // for strcpy.
// #include "d:\qcwin\include\stdlib.h" // for itoa.
// #include "d:\qcwin\include\string.h" // for strcpy.
#include "memory.h"                     // For memset.
#include "t3.h"                         // specific to this program
#include "utl.h"                        // some utilities.
// #include "base.h"                       // Debugging macros.


// Lots of globals!
char    szAppName[] = "BitmapTest1";        // Application name for lpszClassName in WNDCLASS.
char    szBabyName[] = "BabyWindow";        // Baby Window name for lpszClassName in WNDCLASS.
char    szStatusName[] = "StatusWindow";    // Status Window name for lpszClassName in WNDCLASS.
char    szStatusMessage[45];                // Status "Message".
char    szPlayerStatMsg[2][45];             // Put the status messages for players in here.
HANDLE  hInst;                              // current instance
HWND    hWnd, hStatusWnd;                   // Main window handle, Status window handle.
HBITMAP hObitmap, hXbitmap, hCurrentBitmap, hPlayingGrid; // Don't loose this value!
BITMAP  GridBitmap;                         // Put "O" and "X" bitmaps in here
GRID    g;                                  // Player Information, structure.
BOOL    bPlayerMark = 1, bGameNotOver = 0;  // Start with "O" bitmap, GAME STARTS AS OVER.
int     nNumClicks  = 0;                    // Blank the number of clicks.

/****************************************************************************
*
* WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  WinMain is the entry point for windows to hook into, this is the main
*  message loop.
*
****************************************************************************/

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    HANDLE hAccel;                          // for accelerator keys.

    if (!hPrevInstance)                     // Other instances of app running?
    if (!InitApplication(hInstance))        // Initialize shared things
        return (FALSE);                     // Exits if unable to initialize

    // Perform initializations that apply to a specific instance
    if (!InitInstance(hInstance, nCmdShow))
    return (FALSE);

    hAccel = LoadAccelerators( hInstance, "TicMenu" );

    // Acquire and dispatch messages until a WM_QUIT message is received.
    while ( GetMessage( &msg, NULL, NULL, NULL ) )
    {
        // Check to see if an accelerator was pressed.
        if ( !TranslateAccelerator( hWnd, hAccel, &msg ) ) {
            TranslateMessage(&msg);             // Translates virtual key codes
            DispatchMessage(&msg);              // Dispatches message to window
        }
    }
    return (msg.wParam);                // Returns the value from PostQuitMessage
}


/****************************************************************************
*
* InitApplication(HANDLE)
*
*   Initialize window data and registers window class
*
****************************************************************************/

BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the
       main window.                                                           */

    wc.style         = NULL;                // Class style(s).
    wc.lpfnWndProc   = MainWndProc;         // Function to retrieve messages for
                                            // windows of this class.
    wc.cbClsExtra    = 0;                   // No per-class extra data.
    wc.cbWndExtra    = 0;                   // No per-window extra data.
    wc.hInstance     = hInstance;           // Application that owns the class.
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE( IDI_T3ICON ) );
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    // wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName  =  "TicMenu";          // Name of menu resource in .RC file.
    wc.lpszClassName = (LPSTR) szAppName;   // Name used in call to CreateWindow.

    // Register the window class and return success/failure code.

    if ( !RegisterClass(&wc) )
        return FALSE;

    // Register the Baby window(Child).
    wc.lpfnWndProc   = BabyWndProc;
    wc.cbWndExtra    = sizeof (WORD);
    wc.hIcon         = NULL;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    // wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = (LPSTR) szBabyName;

    if ( !RegisterClass(&wc) )
        return FALSE;

    // Register the Status window(Child).
    wc.lpfnWndProc   = StatusWndProc;
    wc.cbWndExtra    = sizeof (WORD);
    wc.hIcon         = NULL;
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    // wc.hbrBackground = GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = (LPSTR) szStatusName;

    if ( !RegisterClass(&wc) )
        return FALSE;

    return TRUE;

}


/****************************************************************************
*
* InitInstance(HANDLE, int)
*
*   Save instance handle and create main window
*
****************************************************************************/

BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    // HWND            hWnd;               // Main window handle.

    /* Save the instance handle in static variable, which will be used in
       many subsequence calls from this application to Windows.            */

    hInst = hInstance;

    // Create a main window for this application instance.
    hWnd = CreateWindow(
        (LPSTR) szAppName,              // See RegisterClass() call.
        "Tic-Tac-Toe",                  // Text for window title bar.
        WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX, // Window style.
        CW_USEDEFAULT,                  // Default horizontal position.
        CW_USEDEFAULT,                  // Default vertical position.
        T3_WIDTH,                       // Default width.
        T3_HEIGHT,                      // Default height.
        NULL,                           // Overlapped windows have no parent.
        NULL,                           // Use the window class menu.
        hInstance,                      // This instance owns this window.
        NULL                            // Pointer not needed.
        );

    // DEBUG( "Size of the caption is %d\n", GetSystemMetrics( SM_CYCAPTION ) )

    // If window could not be created, return "failure"

    if (!hWnd)
    return (FALSE);

    // Make the window visible; update its client area; and return "success"
    ShowWindow(hWnd, nCmdShow);  // Show the window
    UpdateWindow(hWnd);          // Sends WM_PAINT message
    return (TRUE);               // Returns the value from PostQuitMessage

}



/****************************************************************************
*
* MainWndProc(HWND, UINT, WPARAM, LPARAM)
*
*   Processes messages for the main window
*
****************************************************************************/

long FAR PASCAL MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
FARPROC lpProcAbout, lpProcNewGame;     // pointer to the "About" function
// HDC hDC;
static HWND hBaby[9];
PAINTSTRUCT ps;
RECT lprectWinCoord;
int x;
char szTmpBuffer[60];

    switch (message) {

        case WM_CREATE:

            g.hPlayerMark[0]    = LoadBitmap( hInst, MAKEINTRESOURCE(O_BITMAP) );
            g.hPlayerMark[1]    = LoadBitmap( hInst, MAKEINTRESOURCE(X_BITMAP) );
            hPlayingGrid        = LoadBitmap( hInst, MAKEINTRESOURCE(GRID_BIT) );

            // Set the initial "Player Mark" equal to the "O" players
            // bitmap.
            hCurrentBitmap = g.hPlayerMark[0];

            // Create the baby windows.(Grids)
            for ( x=0; x < 9; x++ ) {
                hBaby[x] = CreateWindow( (LPSTR) szBabyName, NULL, WS_CHILD | WS_VISIBLE,
                                     pos[x].horz, pos[x].vert, 50, 50, hWnd,
                                     x, hInst, NULL );
            }

            hStatusWnd   = CreateWindow( (LPSTR) szStatusName, NULL, WS_CHILD | WS_VISIBLE,
                                    10, 250, 221, 21, hWnd,
                                     NULL, hInst, NULL );

            strcpy(szTmpBuffer, NEW_GAME_STRING );
            DisplayInStatus( szTmpBuffer );
            break;
        case WM_PAINT:

            BeginPaint( hWnd, &ps );

            // Paint the playing grid on the screen.
            BlitIt( ps.hdc, hPlayingGrid );

            EndPaint( hWnd, &ps );

            break;
        case WM_COMMAND:

            switch(wParam) {

                case IDM_NEW:
                    lpProcNewGame = MakeProcInstance( NewGame, hInst );

                    DialogBox( hInst, "NewGame", hWnd, lpProcNewGame );

                    FreeProcInstance( lpProcNewGame );
                    break;

                case IDM_EXIT:
                    PostQuitMessage(0);
                    break;

                case IDM_ABOUT:
                    lpProcAbout = MakeProcInstance( About, hInst );

                    DialogBox( hInst, "AboutBox", hWnd, lpProcAbout );

                    FreeProcInstance( lpProcAbout );
                    break;

                default:
                    return ( DefWindowProc( hWnd, message, wParam, lParam ) );
            }

            break;

        case WM_DESTROY:                  /* message: window being destroyed */
            PostQuitMessage(0);
            break;

        default:                          /* Passes it on if unproccessed    */
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}



/************************************************************************************
*
* BabyWndProc: Process all messages for the nine BABY windows in the Tic-Tac-Toe grid
*
*************************************************************************************/

long FAR PASCAL BabyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// HDC hDC;
PAINTSTRUCT bps;
static BOOL bGridTaken, bTmpPlayerNum;
static unsigned nGridNumber;
char szTmpBuffer[80];

    switch (message)
        {

            case WM_LBUTTONUP:

                nGridNumber = GetMenu( hWnd );                      // Get the Baby Windows ID number.

                bGridTaken = bIsGridTaken( nGridNumber );           // Check to see if this grid is taken.

                // if the grid is NOT taken and the Game isn't declared
                // over then put a mark in the players grid.
                if ( !bGridTaken && bGameNotOver ) {
                    g.nGrid[nGridNumber] = bPlayerMark;             // Place a mark in the players grid array.
                    hCurrentBitmap = g.hPlayerMark[bPlayerMark - 1];    // X or O bitmap.
                    InvalidateRect( hWnd, NULL, FALSE );
                    UpdateWindow(hWnd);          // Sends WM_PAINT message
                    nNumClicks++;                                   // Number of clicks as of NOW!

                    if ( nNumClicks >= 5)
                        CheckForWinner( bPlayerMark, hWnd );

                    bPlayerMark = (bPlayerMark == 2) ? 1 : 2;       // If bPlayerMark is 1 make it 2 and vice-versa.

                    if (bGameNotOver)
                    	strcpy( szTmpBuffer, szPlayerStatMsg[bPlayerMark - 1] );
                    else
                        strcpy( szTmpBuffer, NEW_GAME_STRING );

                    DisplayInStatus( szTmpBuffer );

                }
                break;

            case WM_PAINT:

            	nGridNumber = GetMenu( hWnd );
                bGridTaken = bIsGridTaken( nGridNumber );           // Check to see if this grid is taken.

                if (bGridTaken) {
                	BeginPaint( hWnd, &bps );

                    hCurrentBitmap = g.hPlayerMark[ g.nGrid[nGridNumber] - 1 ];

                    // Blast the players mark to the screen, X or O!
                    BlitIt( bps.hdc, hCurrentBitmap );

                    EndPaint( hWnd, &bps );
            	}

                return DefWindowProc( hWnd, message, wParam, lParam );
            	break;

            default:
                return DefWindowProc( hWnd, message, wParam, lParam);
        }

    return 0L;
}

/************************************************************************************
*
* StatusWndProc: Process all messages for the Status Window.
*
*************************************************************************************/

long FAR PASCAL StatusWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// HDC hDCstatus;
PAINTSTRUCT sps;
RECT src;
LOGFONT lf;
HFONT hfontHelv;

    switch (message)
        {

            case WM_T3STATUS:

                // DEBUG("Receiving WM_T3STATUS message\n",0)
                InvalidateRect( hWnd, NULL, FALSE );
                UpdateWindow( hWnd );
                break;

            case WM_PAINT:

                // DEBUG("Receiving WM_PAINT for StatusWndProc\n",0)
                // DEBUG("**** szStatusMessage inside StatusWndProc is %s\n",(LPSTR) szStatusMessage )

                BeginPaint( hWnd, &sps );

                src.left   = 0;// 3
                src.top    = 0;// 2
                src.right  = 220; //217
                src.bottom = 19; //14

                nDrawConcaveFrame( sps.hdc, src );

                memset( &lf, 0, sizeof( LOGFONT ) );
                lf.lfHeight = 9;
                lf.lfWidth = 1;
                lstrcpy( lf.lfFaceName, "Helv" );
                hfontHelv = CreateFontIndirect( &lf );
                SelectObject(sps.hdc, hfontHelv );

                SetBkColor( sps.hdc, clrLTGRAY );

                TextOut( sps.hdc, 5, 4, (LPSTR) szStatusMessage, strlen(szStatusMessage) );

                EndPaint( hWnd, &sps );
                DeleteObject( hfontHelv );
                return DefWindowProc( hWnd, message, wParam, lParam );
                break;

            default:
                return DefWindowProc( hWnd, message, wParam, lParam);
        }
    return 0L;
}

/****************************************************************************
*
* About(HWND, unsigned, WORD, LONG)
*
*   Processes messages for "About" dialog box
*
****************************************************************************/

BOOL FAR PASCAL About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    lParam = lParam;

    switch (message) {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:
            if (wParam == IDOK || wParam == IDCANCEL) {
                EndDialog(hDlg, TRUE);
                return (TRUE);
            }
            break;
    }
    return (FALSE);
}

/****************************************************************************
*
* NewGame(HWND, unsigned, WORD, LONG)
*
*   Processes messages for "New Game..." dialog box
*
****************************************************************************/

BOOL FAR PASCAL NewGame( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
HWND hParent;
char szTmpBuffer[80];

    switch (message) {
        case WM_INITDIALOG:
            return (TRUE);

        case WM_COMMAND:

            if (wParam == IDOK) {

                vResetGridStruct();                     // Reset the grid structure.
                GetDlgItemText( hDlg, IDD_PLAYER1, (LPSTR) g.szPlayerName[0], 30 );
                GetDlgItemText( hDlg, IDD_PLAYER2, (LPSTR) g.szPlayerName[1], 30 );
                EndDialog(hDlg, TRUE);                  // Exits the dialog box

                DoPlayersHaveNames();

                hParent = GetParent( hDlg );            // Get the main windows handle.
                InvalidateRect( hParent, NULL, FALSE ); // This forces a repaint!

                vPlayerStatMsg( g );
                // strcpy(szTmpBuffer, g.szPlayerName[0]);  // Put player #1 in the temp buffer.
                // strcat(szTmpBuffer, "'s turn");          // append to the name, so we get a message.
                strcpy( szTmpBuffer, szPlayerStatMsg[0] );
                DisplayInStatus( szTmpBuffer );         // Set the display string, and send a message.
                bGameNotOver = -1;                      // Tell TTT the game is NOT over.
                return (TRUE);

            } else if (wParam == IDCANCEL) {

                EndDialog(hDlg, TRUE);
                return (TRUE);

            }
            break;
    }

    return (FALSE);
}


/****************************************************************************
*
* bIsGridTaken
*
*   Bit Blit the bitmap specified in the hBitmap parameter to the
*   device specified by the hDC parameter.
*
*****************************************************************************/
BOOL bIsGridTaken( unsigned nGridNumber )
{

    // See if somebody has the "nGridNumber" position in the playing grid.
    if ( g.nGrid[nGridNumber] )
        return TRUE;

return FALSE;                                       // Somebody owns it!
}


/****************************************************************************
*
* CheckForWinner
*
*   Runs through the g.nGrid array to see if we have a winner!
*
*****************************************************************************/
void CheckForWinner( BOOL bPlayerID, HWND hWnd )
{
char szTextBuffer[80];
int nRow, nCol, bWinnerFlag = 0;

    wsprintf( szTextBuffer, "Congratulations %s you win!", (LPSTR) g.szPlayerName[bPlayerID - 1] );

    // Check the Rows for a winner
    for (nRow=0; nRow < 9; nRow +=3) {
        for (nCol = nRow; nCol < (nRow + 3); nCol++) {
            if (g.nGrid[nCol] == bPlayerID) {
                bWinnerFlag++;
                if (bWinnerFlag == 3) {
                    MessageBox(hWnd, szTextBuffer, "Tic-Tac-Toe", MB_OK | MB_ICONEXCLAMATION);
                    // DEBUG("Row number %d is the winner\n",nCol)
                    bGameNotOver = 0;
                    return;
                }
            } else {
                bWinnerFlag = 0;
                break;
            }
        }
    }

    // Check the columns for a winner
    for (nRow = 0; nRow < 3; nRow++) {
        for (nCol = nRow; nCol < (nRow + 9); nCol +=3) {
            if (g.nGrid[nCol] == bPlayerID) {
                bWinnerFlag++;
                if (bWinnerFlag == 3) {
                    MessageBox(hWnd, szTextBuffer, "Tic-Tac-Toe", MB_OK | MB_ICONEXCLAMATION);
                    // DEBUG("Column number %d is the winner\n",nRow)
                    bGameNotOver = 0;
                    return;
                }
            } else {
                bWinnerFlag = 0;
                break;
            }
        }
    }

    // Check for Left diagonal winner
    for (nRow = 0; nRow < 12; nRow += 4) {
        if (g.nGrid[nRow] == bPlayerID) {
            bWinnerFlag++;
            if (bWinnerFlag == 3) {
                MessageBox(hWnd, szTextBuffer, "Tic-Tac-Toe", MB_OK | MB_ICONEXCLAMATION);
                // DEBUG("left diagonal is the winner\n",0)
                bGameNotOver = 0;
                return;
            }
        } else {
            bWinnerFlag = 0;
            break;
        }
    }

    // Check for Right diagonal winner
    for (nRow = 2; nRow < 8; nRow += 2) {
        if (g.nGrid[nRow] == bPlayerID) {
            bWinnerFlag++;
            if (bWinnerFlag == 3) {
                MessageBox(hWnd, szTextBuffer, "Tic-Tac-Toe", MB_OK | MB_ICONEXCLAMATION);
                // DEBUG("right diagonal is the winner\n",0)
                bGameNotOver = 0;
                return;
            }

        } else {
            bWinnerFlag = 0;
            break;

        }
    }

    if (nNumClicks >= 9) {
        bGameNotOver = 0;
        MessageBox(hWnd, "We have a tie game!", "Tic-Tac-Toe", MB_OK | MB_ICONEXCLAMATION);
    }

}


/****************************************************************************
*
* DisplayInStatus
*
*       Stuffs the string szDisplayString into the global string
*       szStatusMessage then sends a message to StatusWndProc to display
*       the string.
*
****************************************************************************/

void DisplayInStatus( char *szDisplayString )
{
    // DEBUG( "Status Message is %s\n", (LPSTR) szDisplayString )
    // DEBUG( "Lenght of string is %d\n", strlen( szDisplayString ) )

    strcpy( szStatusMessage, szDisplayString );
    vPadString( szStatusMessage, 45 );
    SendMessage (hStatusWnd, WM_T3STATUS, 0, 0L );

}

/****************************************************************************
*
* vResetGridStruct
*
*       Reset the g.nGrid elements to zero, set the initial player mark back
*       to 1, reset the nNumClicks variable back to 0, so we can get
*       ready for the next game.
*
*****************************************************************************/
void vResetGridStruct(void)
{
    int nArrayElement;

    for(nArrayElement=0; nArrayElement < 9; nArrayElement++) {
        g.nGrid[nArrayElement] = 0;
    }
    bPlayerMark = 1;
    nNumClicks  = 0;

}





void DoPlayersHaveNames( )
{

    if ( !strlen(g.szPlayerName[0]) | g.szPlayerName[0][0] == ' ' )
        strcpy( g.szPlayerName[0], "Player One" );



    if ( !strlen(g.szPlayerName[1]) | g.szPlayerName[1][0] == ' ' )
        strcpy( g.szPlayerName[1], "Player Two" );


}


void vPlayerStatMsg( GRID g )
{
    int x = 0, y = 0;

    strcpy( szPlayerStatMsg[0], g.szPlayerName[0] );
    strcpy( szPlayerStatMsg[1], g.szPlayerName[1] );

    x = ( strlen( szPlayerStatMsg[0] ) - 1 );
    y = ( strlen( szPlayerStatMsg[1] ) - 1 );

    // Setup the first player status message.
    if ( szPlayerStatMsg[0][x] == 's' )
        strcat( szPlayerStatMsg[0], S_END );
    else if ( szPlayerStatMsg[0][x] == 'x' )
        strcat( szPlayerStatMsg[0], X_END );
    else
        strcat( szPlayerStatMsg[0], N_END );


    // Get the Second Player Status Message setup
    if ( szPlayerStatMsg[1][y] == 's' )
        strcat( szPlayerStatMsg[1], S_END );
    else if ( szPlayerStatMsg[1][y] == 'x' )
        strcat( szPlayerStatMsg[1], X_END );
    else
        strcat( szPlayerStatMsg[1], N_END );

}


// Resource I.D.'s for the bitmaps used.
#define O_BITMAP 10
#define X_BITMAP 11
#define GRID_BIT 12
#define IDI_T3ICON 14

// I.D.'s for the "File" menu and the "New Game..." and "Exit" commands.
#define FileMenu	100
#define IDM_NEW		110
#define IDM_EXIT	120

// I.D.'s for the "Help" menu and the "About" command.
#define HelpMenu	200
#define IDM_ABOUT	210

// I.D.'s for the "New Game..." dialog box.
#define IDD_PLAYER1 301
#define IDD_PLAYER2 302

// Height of the player status window.
#define STATUS_WIN_HEIGHT 10

// My own message for "StatusWndProc", send this message when you want to
// display, or change, text in the status window.
#define WM_T3STATUS WM_USER+1


// Just some other Misc. defines.
#define NEW_GAME_STRING "Press F2 to start a new game."
#define S_END           "' turn."
#define X_END			"'es turn."
#define N_END			"'s turn."

// Just a couple of macros to get the width and height of the window right.
#define T3_WIDTH ( 240 + (GetSystemMetrics( SM_CXBORDER ) + 1) )
#define T3_HEIGHT ( 250 + (GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU ) + 26 ) )

// The grid structure.
typedef struct {
	int nGrid[9];							// Mirror the grid, determines who is the winner!
	HBITMAP hPlayerMark[2];					// Handle to Player Bitmaps.
	char szPlayerName[2][31];				// Player names.
} GRID;

// This will tell where to put the Baby Windows.
typedef struct {
	int horz;
	int vert;
} POSITION;

                          // h  v
static POSITION pos[9] = { 	{25,25},     	// top left
                           	{95,25},     	// top middle
			   			   	{165,25},    	// top right
		           			{25,98},    	// middle left
			   				{95,98},    	// middle middle
			   				{165,98},   	// middle right
			   				{25,172},    	// bottom left
			   				{95,172},    	// bottom middle
			   				{165,172} };	// bottom right


// Function prototypes.
int PASCAL WinMain( HANDLE, HANDLE, LPSTR, int );
BOOL InitApplication( HANDLE );
BOOL InitInstance( HANDLE, int );
long FAR PASCAL MainWndProc( HWND, UINT, WPARAM, LPARAM );
long FAR PASCAL BabyWndProc( HWND, UINT, WPARAM, LPARAM );
long FAR PASCAL StatusWndProc( HWND, UINT, WPARAM, LPARAM );
BOOL FAR PASCAL NewGame( HWND, UINT, WPARAM, LPARAM );
BOOL FAR PASCAL About( HWND, unsigned, WORD, LONG );
BOOL bIsGridTaken( unsigned );
void CheckForWinner( BOOL, HWND );
void vResetGridStruct( void );
void DisplayInStatus( char * );
void DoPlayersHaveNames( void );
void vPlayerStatMsg( GRID );

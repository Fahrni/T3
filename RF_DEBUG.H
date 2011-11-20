//------------------------------------------------------------------------
// Some useful debugging macros
//------------------------------------------------------------------------
#ifdef DEBUGGING
char    szDebugBuffer[80];
#define DEBUG(parm1,parm2)\
    wsprintf(szDebugBuffer,parm1,parm2);\
    OutputDebugString(szDebugBuffer);
#else
#define DEBUG(parm1,parm2)
#endif

#ifdef DEBUGMSGS
char    szMsgBuffer[40];
#define DEBUGMSG(parm1)\
    wsprintf(szDebugBuffer,"%s: %s | %04X | %08lX\n\r", (LPSTR) parm1,\
             MessageName(szMsgBuffer,wMsgID),wParam,lParam);\
    OutputDebugString(szDebugBuffer);
#else
#define DEBUGMSG(parm1)
#endif
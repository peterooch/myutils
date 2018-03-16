#include <Windows.h>
#include <string.h>
#include <stdio.h>
int wmain()
{
    WCHAR desc[255];
    int (FAR STDAPICALLTYPE * GetUName)(WORD wCharCode, LPWSTR lpbuf);
    HINSTANCE hgetuname = NULL;
    //getUName GetUName;
    hgetuname = LoadLibraryW(L"getuname.dll");
    if (hgetuname != NULL)
    {
        (FARPROC&)GetUName = /*(getUName)*/GetProcAddress(hgetuname,"GetUName");
        if (GetUName == NULL)
        {
            FreeLibrary(hgetuname);
            return 1;
        }
    }
    WCHAR ch = getwchar();
    int res = GetUName(ch,desc);
    if (desc == NULL)
    {
        wprintf(L"Error %d",res);
        return 1;
    }
    wprintf(L"U+%04X: %s",ch,desc);
    wprintf(L"Press a key to continue");
    getwchar();
    return 0;
}

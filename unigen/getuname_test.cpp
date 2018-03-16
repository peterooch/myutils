#include <Windows.h>
#include <string.h>
#include <stdio.h>

/*
A simple program that demonstrates the GetUName function of the
Windows/ReactOS getuname.dll
This DLL is usually only used by the charmap system application.
*/

int wmain()
{
    WCHAR desc[255];
    int (FAR STDAPICALLTYPE * GetUName)(WORD wCharCode, LPWSTR lpbuf);
    HINSTANCE hGetUName = LoadLibraryW(L"getuname.dll");
    //getUName GetUName;
    if (hGetUName != NULL)
    {
        (FARPROC&)GetUName = /*(getUName)*/GetProcAddress(hGetUName,"GetUName");
        if (GetUName == NULL)
        {
            FreeLibrary(hGetUName);
            return 1;
        }
    }
    wprintf(L"Please type a character:");
    WCHAR ch = getwchar();
    int res = GetUName(ch,desc);
    if (desc == NULL)
    {
        wprintf(L"Error %d",res);
        return 1;
    }
    wprintf(L"U+%04X: %s",ch,desc);
    wprintf(L"Press return to exit.");
    getwchar();
    return 0;
}

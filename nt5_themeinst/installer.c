#include <windows.h>
#include <strsafe.h>

static BOOL InstallTheme(LPCWSTR theme_ini)
{
    WCHAR name[MAX_PATH] = { 0 };
    WCHAR copy_dir[MAX_PATH] = { 0 };
    WCHAR msstyle_name[MAX_PATH] = { 0 };
    WCHAR msstyle_dest[MAX_PATH] = { 0 };

    GetPrivateProfileStringW(L"ThemeData", L"ThemeName", NULL, name, MAX_PATH, theme_ini);
    GetPrivateProfileStringW(L"ThemeData", L"ThemeFile", NULL, msstyle_name, MAX_PATH, theme_ini);

    if (wcscmp(name, L"") == 0 || wcscmp(msstyle_name, L"") == 0)
        return FALSE;

    if (!GetWindowsDirectoryW(copy_dir, MAX_PATH))
    {
        MessageBoxW(NULL, L"GetWindowsDirectoryW failed", L"Error", MB_ICONSTOP);
        return FALSE;
    }

    StringCchCatW(copy_dir, MAX_PATH, L"\\Resources\\Themes\\");
    StringCchCatW(copy_dir, MAX_PATH, name);

    if (!CreateDirectoryW(copy_dir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        MessageBoxW(NULL, L"CreateDirectoryW failed", L"Error", MB_ICONSTOP);
        return FALSE;
    }

    StringCchCopyW(msstyle_dest, MAX_PATH, copy_dir);
    StringCchCatW(msstyle_dest, MAX_PATH, L"\\");
    StringCchCatW(msstyle_dest, MAX_PATH, msstyle_name);

    if (!CopyFileW(msstyle_name, msstyle_dest, FALSE))
    {
        MessageBoxW(NULL, L"CopyFileW failed", L"Error", MB_ICONSTOP);
        return FALSE;
    }

    MessageBoxW(NULL, L"Theme copying completed successfully.", L"Theme Copied", MB_ICONINFORMATION);
    return TRUE;
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nShowCmd)
{
    WCHAR sanitizedname[MAX_PATH] = { 0 };
    WCHAR ini_dir[MAX_PATH] = { 0 };
    INT i = 0, j = 0;
    BOOL ok = FALSE; //mystery boolean flag

    UNREFERENCED_PARAMETER(hInst);
    UNREFERENCED_PARAMETER(hPrevInst);
    UNREFERENCED_PARAMETER(nShowCmd);

    while (lpCmdLine[i])
    {
        if (lpCmdLine[i] == L'"' && !ok)
        {
            ok = TRUE;
            i++;
            continue;
        }
        else if (lpCmdLine[i] == L'"')
        {
            ok = FALSE;
            break;
        }

        if (ok)
            sanitizedname[j++] = lpCmdLine[i];

        i++;
    }

    if (j == 0 || ok)
    {
        MessageBoxW(NULL, L"Usage: themeinstaller ""<theme_ini_file>""", L"Invalid parameters", MB_ICONEXCLAMATION);
        return 0;
    }
    sanitizedname[j] = 0;

    if (!GetCurrentDirectoryW(MAX_PATH, ini_dir))
        return 0;

    StringCchCatW(ini_dir, MAX_PATH, L"\\");
    StringCchCatW(ini_dir, MAX_PATH, sanitizedname);

    InstallTheme(ini_dir);

    return 0;
}
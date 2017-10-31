#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QString>
#include <Windows.h>

const QString PATH = "C:/Users/Alexis/Desktop/";
const QString FILE_NAME = "test.exe";
const QString PATH_LINK = "C:/Users/Alexis/Desktop/";

class Shortcut
{
public:
Shortcut();

HRESULT createWindowsShortcut(LPCWSTR pszTargetfile, LPCWSTR pszTargetargs,
                              LPSTR pszLinkfile, LPCWSTR pszDescription,
                              int iShowmode, LPCWSTR pszCurdir,
                              LPCWSTR pszIconfile, int iIconindex);
};

#endif // SHORTCUT_H

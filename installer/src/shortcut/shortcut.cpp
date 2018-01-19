#include "shortcut/shortcut.h"

#ifdef _WIN32

    #include <Windows.h>
    #include <shlobj.h>
    #include <winnls.h>
    #include <shobjidl.h>
    #include <objbase.h>
    #include <objidl.h>
    #include <shlguid.h>
    #include <Shlobj.h>

    #include "log/logger.h"

    Shortcut::Shortcut()
    {
    }

    HRESULT Shortcut::createWindowsShortcut(LPCWSTR _pszTargetfile, LPCWSTR _pszTargetargs,
                                            LPSTR _pszLinkfile, LPCWSTR _pszDescription,
                                            int _iShowmode, LPCWSTR _pszCurdir,
                                            LPCWSTR _pszIconfile, int _iIconindex)
    {
        HRESULT hRes;                       /* Returned COM result code */
        IShellLink * pShellLink;            /* IShellLink object pointer */
        IPersistFile * pPersistFile;        /* IPersistFile object pointer */
        WCHAR wszLinkfile[MAX_PATH];        /* pszLinkfile as Unicode string */

        CoInitialize(NULL);
        hRes = E_INVALIDARG;
        if (
            (_pszTargetfile != NULL) && (lstrlen(_pszTargetfile) > 0) &&
            (_pszTargetargs != NULL) &&
            (_pszLinkfile != NULL) && (strlen(_pszLinkfile) > 0) &&
            (_pszDescription != NULL) &&
            (_iShowmode >= 0) &&
            (_pszCurdir != NULL) &&
            (_pszIconfile != NULL) &&
            (_iIconindex >= 0)
            ) {
            hRes = CoCreateInstance(
                    CLSID_ShellLink,        /* pre-defined CLSID of the IShellLink object */
                    NULL,                   /* pointer to parent interface if part of aggregate */
                    CLSCTX_INPROC_SERVER,   /* caller and called code are in same process */
                    IID_IShellLink,         /* pre-defined interface of the IShellLink object */
                    (LPVOID *)&pShellLink); /* Returns a pointer to the IShellLink object */

            if (SUCCEEDED(hRes)) {
                /* Set the fields in the IShellLink object */
                hRes = pShellLink->SetPath(_pszTargetfile);
                hRes = pShellLink->SetArguments(_pszTargetargs);
                if (lstrlen(_pszDescription) > 0) {
                    hRes = pShellLink->SetDescription(_pszDescription);
                }
                if (_iShowmode > 0) {
                    hRes = pShellLink->SetShowCmd(_iShowmode);
                }
                if (lstrlen(_pszCurdir) > 0) {
                    hRes = pShellLink->SetWorkingDirectory(_pszCurdir);
                }
                if (lstrlen(_pszIconfile) > 0 && _iIconindex >= 0) {
                    hRes = pShellLink->SetIconLocation(_pszIconfile, _iIconindex);
                }

                /* Use the IPersistFile object to save the shell link */
                hRes = pShellLink->QueryInterface(
                        IID_IPersistFile,         /* pre-defined interface of the
                                                   *  IPersistFile object */
                        (LPVOID *)&pPersistFile); /* returns a pointer to the
                                                   * IPersistFile object */
                if (SUCCEEDED(hRes)) {
                    MultiByteToWideChar(CP_ACP, 0,
                                        _pszLinkfile, -1,
                                        wszLinkfile, MAX_PATH);
                    hRes = pPersistFile->Save(wszLinkfile, TRUE);
                    pPersistFile->Release();
                }
                pShellLink->Release();
            }
        }
        CoUninitialize();
        return (hRes);
    }

    QString Shortcut::findAllUserStartMenuProgramsFolder()
    {
         return findWindowsPath(CSIDL_COMMON_PROGRAMS);
    }

    QString Shortcut::findAllUserStartupFolder()
    {
        return findWindowsPath(CSIDL_COMMON_STARTUP);
    }

    QString Shortcut::findUserStartMenuProgramFolder()
    {
         return findWindowsPath(CSIDL_PROGRAMS);
    }

    QString Shortcut::findUserStartupFolder()
    {
         return findWindowsPath(CSIDL_STARTUP);
    }

    QString Shortcut::findWindowsPath(int hToken)
    {
        WCHAR path[MAX_PATH];
        HRESULT hr = SHGetFolderPathW(NULL, hToken, NULL, 0, path);
        if (SUCCEEDED(hr)) {
            QString p = QString::fromWCharArray(path);
            return p;
        }
        else{
            L_INFO("Impossible to find windows path with handle = " + QString::number(hToken));
        }
        return "";
    }

#endif

#include "shortcut.h"
#include <Windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>

Shortcut::Shortcut(){
}

/**
 * @brief Creates the actual 'lnk' file (assumes COM has been initialized).
 *
 * @param pszTargetfile     - File name of the link's target.
 * @param pszTargetargs     - Command line arguments passed to link's target.
 * @param pszLinkfile       - File name of the actual link file being created.
 * @param pszDescription    - Description of the linked item.
 * @param iShowmode         - ShowWindow() constant for the link's target.
 * @param pszCurdir         - Working directory of the active link.
 * @param pszIconfile       - File name of the icon file used for the link.
 * @param iIconindex        - Index of the icon in the icon file.
 *
 * @return HRESULT value >= 0 for success, < 0 for failure.
 */
HRESULT Shortcut::createWindowsShortcut(LPCWSTR pszTargetfile, LPCWSTR pszTargetargs,
                                        LPSTR pszLinkfile, LPCWSTR pszDescription,
                                        int iShowmode, LPCWSTR pszCurdir,
                                        LPCWSTR pszIconfile, int iIconindex){
    HRESULT hRes;                       /* Returned COM result code */
    IShellLink * pShellLink;            /* IShellLink object pointer */
    IPersistFile * pPersistFile;        /* IPersistFile object pointer */
    WCHAR wszLinkfile[MAX_PATH];        /* pszLinkfile as Unicode string */

    CoInitialize(NULL);
    hRes = E_INVALIDARG;
    if (
        (pszTargetfile != NULL) && (lstrlen(pszTargetfile) > 0) &&
        (pszTargetargs != NULL) &&
        (pszLinkfile != NULL) && (strlen(pszLinkfile) > 0) &&
        (pszDescription != NULL) &&
        (iShowmode >= 0) &&
        (pszCurdir != NULL) &&
        (pszIconfile != NULL) &&
        (iIconindex >= 0)
        ) {
        hRes = CoCreateInstance(
                CLSID_ShellLink,        /* pre-defined CLSID of the IShellLink object */
                NULL,                   /* pointer to parent interface if part of aggregate */
                CLSCTX_INPROC_SERVER,   /* caller and called code are in same process */
                IID_IShellLink,         /* pre-defined interface of the IShellLink object */
                (LPVOID *)&pShellLink); /* Returns a pointer to the IShellLink object */

        if (SUCCEEDED(hRes)) {
            /* Set the fields in the IShellLink object */
            hRes = pShellLink->SetPath(pszTargetfile);
            hRes = pShellLink->SetArguments(pszTargetargs);
            if (lstrlen(pszDescription) > 0) {
                hRes = pShellLink->SetDescription(pszDescription);
            }
            if (iShowmode > 0) {
                hRes = pShellLink->SetShowCmd(iShowmode);
            }
            if (lstrlen(pszCurdir) > 0) {
                hRes = pShellLink->SetWorkingDirectory(pszCurdir);
            }
            if (lstrlen(pszIconfile) > 0 && iIconindex >= 0) {
                hRes = pShellLink->SetIconLocation(pszIconfile, iIconindex);
            }

            /* Use the IPersistFile object to save the shell link */
            hRes = pShellLink->QueryInterface(
                    IID_IPersistFile,         /* pre-defined interface of the
                                               *  IPersistFile object */
                    (LPVOID *)&pPersistFile); /* returns a pointer to the
                                               * IPersistFile object */
            if (SUCCEEDED(hRes)) {
                MultiByteToWideChar(CP_ACP, 0,
                                    pszLinkfile, -1,
                                    wszLinkfile, MAX_PATH);
                hRes = pPersistFile->Save(wszLinkfile, TRUE);
                pPersistFile->Release();
            }
            pShellLink->Release();
        }
    }
    CoUninitialize();
    return (hRes);
} // Shortcut::Create

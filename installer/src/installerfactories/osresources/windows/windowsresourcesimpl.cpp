#include "installerfactories/osresources/windows/windowsresourcesimpl.h"

WindowsResourcesImpl::WindowsResourcesImpl(const QString &_installPath) : OsResourcesImpl(_installPath),
    m_installPath(_installPath)
{
}

bool WindowsResourcesImpl::extractResources()
{
    BOOL res = EnumResourceNames(NULL, RT_RCDATA, (ENUMRESNAMEPROC)&WindowsResourcesImpl::EnumNamesFunc, (LONG_PTR)&m_installPath);

    if (!res) {
        long status = GetLastError();
        L_ERROR(QString("Unable to call EnumResourcesNames. Error code: %1").arg(QString::number(status)));
    }

    return res != 0;
}

bool WindowsResourcesImpl::extractProjectIniToTempFile(const QString &_toPath)
{
    return writeResourceIdToFile(L"_PROJECT__INI", _toPath);
}

bool WindowsResourcesImpl::extractStyleCssToTempFile(const QString &_toPath)
{
    return writeResourceIdToFile(L"_STYLE__CSS", _toPath);
}
bool WindowsResourcesImpl::extractTitlePngToTempFile(const QString &_toPath)
{
    return writeResourceIdToFile(L"IMAGES_TITLE__PNG", _toPath);
}

BOOL WindowsResourcesImpl::EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam)
{
    Q_UNUSED(hModule)
    Q_UNUSED(lpType)

    QString resourceName = QString::fromStdWString(lpName);
    L_INFO(QString("Read resource name: %1").arg(resourceName));
    if (!resourceName.startsWith("_")) {
        resourceName.replace("__", ".");
        resourceName.replace("_", QDir::separator());

        const QString processedResourceName = resourceName.toLower();
        L_INFO(QString("Processed resource name: %1").arg(processedResourceName));

        if (lParam != 0) {
            const QString installPath(*(QString *)lParam);
            L_INFO(QString("Resulting file: %1\\%2").arg(installPath).arg(processedResourceName));
            return WindowsResourcesImpl::writeResourceIdToFile(lpName, QString("%1\\%2").arg(installPath).arg(processedResourceName)) ? TRUE : FALSE;
        } else {
            L_ERROR(QString("Install Path param is NULL. Unable to extract resource for: %1.").arg(processedResourceName));
            return FALSE;
        }
    } else {
        L_INFO(QString("Ignoring resource: %1").arg(resourceName));
        return TRUE;
    }
}

bool WindowsResourcesImpl::writeResourceIdToFile(LPCWSTR _resId, const QString &_toPath)
{
    HGLOBAL res_handle = nullptr;
    HRSRC res;
    char * res_data;
    DWORD res_size;

    res = FindResource(GetModuleHandle(nullptr), _resId, RT_RCDATA);
    if (!res) {
        L_ERROR("FindResource failed.");
        return false;
    }

    res_handle = LoadResource(nullptr, res);
    if (!res_handle) {
        L_ERROR("LoadResource failed.");
        return false;
    }

    res_data = (char *)LockResource(res_handle);
    res_size = SizeofResource(nullptr, res);

    const QByteArray bytes(res_data, res_size);

    const QFileInfo fileInfo(_toPath);
    const QDir parentDir = fileInfo.dir();
    const bool created = FileUtils::createDirIfNotExists(parentDir);

    if (created) {
        QFile file(_toPath);
        if (file.open(QIODevice::WriteOnly)) {
            bool result;
            if (file.write(bytes) == -1) {
                L_ERROR(QString("Unable to write file %1").arg(_toPath));
                result = false;
            } else {
                L_INFO(QString("File %1 written.").arg(_toPath));
                result = true;
            }

            file.close();
            return result;
        } else {
            L_ERROR(QString("Unable to open file %1 for write.").arg(_toPath));
        }
    }

    return false;
}

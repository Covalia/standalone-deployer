#include "manager/resources/windowsresources.h"

WindowsResources::WindowsResources(const QString _installPath) :
    m_installPath(_installPath) {
}

WindowsResources::~WindowsResources() {
}

bool WindowsResources::extractResources()
{
    BOOL res = EnumResourceNames(NULL, RT_RCDATA, (ENUMRESNAMEPROC)&WindowsResources::EnumNamesFunc, (LONG_PTR) &m_installPath);

    if (!res) {
        long status = GetLastError();
        L_ERROR(QString("Unable to call EnumResourcesNames. Error code: %1").arg(QString::number(status)));
    }

    return res != 0;
}

bool WindowsResources::extractProjectIniToTempFile(const QString &_path)
{
    return writeResourceIdToFile(L"_PROJECT__INI", _path);
}

bool WindowsResources::extractStyleCssToTempFile(const QString &_path)
{
    return writeResourceIdToFile(L"CONFIG_STYLE__CSS", _path);
}
bool WindowsResources::extractTitlePngToTempFile(const QString &_path)
{
    return writeResourceIdToFile(L"IMAGES_TITLE__PNG", _path);
}

BOOL WindowsResources::EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam)
{
    Q_UNUSED(hModule);
    Q_UNUSED(lpType);

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
            return WindowsResources::writeResourceIdToFile(lpName, QString("%1\\%2").arg(installPath).arg(processedResourceName)) ? TRUE : FALSE;
        }
        else {
            L_ERROR(QString("Install Path param is NULL. Unable to extract resource for: %1.").arg(processedResourceName));
            return FALSE;
        }
    }
    else {
        L_INFO(QString("Ignoring resource: %1").arg(resourceName));
        return TRUE;
    }
}

bool WindowsResources::writeResourceIdToFile(LPCWSTR _resId, const QString &_path)
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

    res_data = (char *) LockResource(res_handle);
    res_size = SizeofResource(nullptr, res);

    const QByteArray bytes(res_data, res_size);

    const QFileInfo fileInfo(_path);
    const QDir parentDir = fileInfo.dir();
    const bool created = FileUtils::createDirIfNotExists(parentDir);

    if (created) {
        QFile file(_path);
        if (file.open(QIODevice::WriteOnly)) {

            bool result;
            if (file.write(bytes) == -1) {
                L_ERROR(QString("Unable to write file %1").arg(_path));
                result = false;
            }
            else {
                L_INFO(QString("File %1 written.").arg(_path));
                result = true;
            }

            file.close();
            return result;
        } else {
            L_ERROR(QString("Unable to open file %1 for write.").arg(_path));
        }
    }

    return false;
}

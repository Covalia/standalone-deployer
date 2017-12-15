#ifndef DEPLOYMENTXML_H
#define DEPLOYMENTXML_H

#include <QString>
#include <QXmlStreamReader>
#include <QList>
#include <QMap>
#include <QFile>
#include <QDebug>
#include "xml/download.h"
#include "app/application.h"
#include "app/javaupdate.h"

/**
 * @class DeploymentXML
 * \brief This class can read a CNLP xml file and extract infromation (java version, file to download etc) into different objects
 */
class DeploymentXML : public QObject
{
    Q_OBJECT

    public:
        /**
         * @brief constructor
         * @param _pathCnlp : path to Cnlp file
         * @param _parent
         */
        explicit DeploymentXML(const QString &_pathCnlp, QObject * _parent = 0);

        /**
         * @brief destructor
         */
        ~DeploymentXML();

        /**
         * @brief start the cnlp analysis
         * @return
         */
        bool read();

        QString getVersion() const;
        QList<QString> getArguments() const;
        QMap<Application, QList<Download> > getApplications() const;
        QList<Download> getDownloads(const Application &_application, const QString &_os) const;
        QMap<QString, JavaUpdate> getJavaUpdates() const;
        QString getMemory() const;

        static const QString RDeploymentTag;
        static const QString DownloadsTag;
        static const QString JarTag;

        static const QString ResourceTag;

        static const QString OsAttribute;
        static const QString OsMacOsValue;
        static const QString OsWindowsValue;
        static const QString OsAnyValue;
        static const QString NativeAttribute;
        static const QString HrefAttribute;
        static const QString HashAttribute;
        static const QString MainAttribute;

        static const QString ApplicationTag;
        static const QString FileTag;

        static const QString VersionAttribute;
        static const QString NameAttribute;

        static const QString DownloaderExtensionClasspathAttribute;

        static const QString ArgumentsTag;
        static const QString ArgumentTag;

        static const QString MemoryTag;
        static const QString VersionTag;

        static const QString JavaTag;
        static const QString JavaVersionAttribute;

    private:

        QFile m_xmlFile;
        QXmlStreamReader m_xmlReader;
        QString m_memory;
        QString m_version;
        QList<QString> m_arguments;
        QMap<Application, QList<Download> > m_applications;
        QMap<QString, JavaUpdate> m_javaUpdates;

        /**
         * @brief read deplyment tag in xml (main tag)
         * @return true if success analysis
         */
        bool processDeployment();

        /**
         * @brief read version tag in xml
         * @return true if success analysis
         */
        bool processVersion();

        /**
         * @brief read memory tag in xml
         * @return true if success analysis
         */
        bool processMemory();

        /**
         * @brief read java tag in xml
         * @return true if success analysis
         */
        bool processJava();

        /**
         * @brief read all arguments tags
         * @return true if success analysis
         */
        bool processArguments();

        /**
         * @brief read one argument tag in xml
         * @return true if success analysis
         */
        bool processArgument();

        /**
         * @brief read all downloads (application list) tag in xml
         * @return true if success analysis
         */
        bool processDownloads();

        /**
         * @brief read one application tag in xml and add download file inforamtion in this application
         * @return true if success analysis
         */
        bool processApplication();

        /**
         * @brief read one download tag in xml (one download = one file to download for a application)
         * @return true if success analysis
         */
        Download processDownload();

        /**
         * @brief read text between current tags
         * @return true if success analysis
         */
        QString readNextText();
};

#endif // ifndef DEPLOYMENTXML_H
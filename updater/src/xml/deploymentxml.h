#ifndef UPDATER_DEPLOYMENTXML_H
#define UPDATER_DEPLOYMENTXML_H

#include <QString>
#include <QXmlStreamReader>
#include <QList>
#include <QFile>
#include <QDebug>
#include "xml/data/download.h"
#include "xml/data/application.h"
#include "xml/data/javaupdate.h"

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
        explicit DeploymentXML(const QString &_pathCnlp, QObject * _parent = nullptr);

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
        Application getApplication() const;
        QList<Download> getDownloads() const;
        QString getMemory() const;
        QString getEncoding() const;
        QString getMainClass() const;
        QString getRunnerClass() const;
        QString getServerName() const;

        static QString getCurrentOsValue();

        static const QString DeploymentTag;
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
        static const QString ClasspathAttribute;

        static const QString ApplicationTag;
        static const QString FileTag;

        static const QString VersionAttribute;
        static const QString NameAttribute;

        static const QString ArgumentsTag;
        static const QString ArgumentTag;

        static const QString MemoryTag;
        static const QString VersionTag;
        static const QString EncodingTag;
        static const QString MainClassTag;
        static const QString RunnerClassTag;
        static const QString ServerNameTag;

    private:

        QFile m_xmlFile;
        QXmlStreamReader m_xmlReader;
        QString m_memory;
        QString m_version;
        QString m_encoding;
        QString m_mainClass;
        QString m_runnerClass;
        QString m_serverName;
        QList<QString> m_arguments;

        Application m_application;
        QList<Download> m_downloads;

        /**
         * @brief read deployment tag in xml (main tag)
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
         * @brief read encoding tag in xml
         * @return true if success analysis
         */
        bool processEncoding();

        /**
         * @brief read mainClass tag in xml
         * @return true if success analysis
         */
        bool processMainClass();

        /**
         * @brief read runnerClass tag in xml
         * @return true if success analysis
         */
        bool processRunnerClass();

        /**
         * @brief read server_name tag in xml
         * @return true if success analysis
         */
        bool processServerName();

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

#endif

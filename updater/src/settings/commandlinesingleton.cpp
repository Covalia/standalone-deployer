#include "settings/commandlinesingleton.h"

std::mutex CommandLineSingleton::sm_mutex;

CommandLineSingleton::CommandLineSingleton()
{
}

CommandLineSingleton::~CommandLineSingleton()
{
}

void CommandLineSingleton::setArguments(const QStringList &_arguments)
{
    m_arguments = QStringList(_arguments);
}

QStringList CommandLineSingleton::getAllArguments() const
{
    return QStringList(m_arguments);
}

QStringList CommandLineSingleton::getApplicationArguments() const
{
    QStringList arguments = QStringList(m_arguments);

    arguments.removeAll("--configure");
    arguments.removeAll("--debug");

    return arguments;
}

QString CommandLineSingleton::getApplicationHttpArguments() const
{
    QStringList arguments = getApplicationArguments();
    QString query = "";
    QString separator = "?";
    QString start = "--";
    QStringListIterator iterator(arguments);
    while (iterator.hasNext()) {
        const QString arg = iterator.next();
        if (arg.startsWith(start)) {
            query += separator;
            query += arg.right(arg.length() - start.length());
            separator = "&";
        }
    }

    return query;
}

bool CommandLineSingleton::isDebugMode() const
{
    return m_arguments.contains("--debug");
}

bool CommandLineSingleton::isConfigureMode() const
{
    return m_arguments.contains("--configure");
}

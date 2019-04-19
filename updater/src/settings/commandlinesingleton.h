#ifndef UPDATER_COMMANDLINESINGLETON_H
#define UPDATER_COMMANDLINESINGLETON_H

#include <mutex>
#include <QStringList>

class CommandLineSingleton {
    public:
        static CommandLineSingleton * getInstance()
        {
            static CommandLineSingleton * factory = nullptr;

            sm_mutex.lock();
            if (!factory) {
                factory = new CommandLineSingleton;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~CommandLineSingleton();

        void setArguments(const QStringList &_arguments);

        /// get all arguments given to application
        QStringList getAllArguments() const;

        /// get arguments entended for application. updater specific arguments are removed from this list.
        QStringList getApplicationArguments() const;

        QString getApplicationHttpArguments() const;

        /// is it debug mode?
        bool isDebugMode() const;

        /// is it configuration mode?
        bool isConfigureMode() const;

        /// is it offline mode?
        bool isOfflineMode() const;

    private:
        CommandLineSingleton();

        static std::mutex sm_mutex;
        QStringList m_arguments;
};

#endif

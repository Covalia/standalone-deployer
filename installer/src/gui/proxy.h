#ifndef PROXY_H
#define PROXY_H

#include <QWidget>

namespace Ui {
class Proxy;
}

/**
 * @class Proxy
 * @brief Proxy widget
 */
class Proxy : public QWidget
{
    Q_OBJECT

    public:
        explicit Proxy(QWidget * _parent = 0);
        virtual ~Proxy();

    private:
        Ui::Proxy * m_ui;

    signals:
        void validateSettingsSignal();

    public slots:
        void changeLanguage();
        void validateSettings();

    private slots:
        void updateVisibleField();
};

#endif // PROXY_H

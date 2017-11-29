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
        explicit Proxy(QWidget * parent = 0);
        ~Proxy();

    private:
        Ui::Proxy * ui;

    signals:
        void validateSettingsSignal();

    public slots:
        void changeLanguage();
        void validateSettings();

    private slots:
        void updateVisibleField();
};

#endif // PROXY_H

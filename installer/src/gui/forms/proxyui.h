#ifndef INSTALLER__PROXY_H
#define INSTALLER__PROXY_H

#include <QWidget>

namespace Ui {
class ProxyUI;
}

/**
 * @class ProxyUI
 * @brief Proxy widget
 */
class ProxyUI : public QWidget
{
    Q_OBJECT

    public:
        explicit ProxyUI(QWidget * _parent = 0);
        virtual ~ProxyUI();

    private:
        Ui::ProxyUI * m_ui;

    signals:
        void validateSettingsSignal();

    public slots:
        void changeLanguage();
        void validateSettings();

    private slots:
        void updateVisibleField();
};

#endif // INSTALLER__PROXY_H

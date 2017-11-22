#ifndef PERSONNALIZE_H
#define PERSONNALIZE_H

#include <QWidget>
#include <QString>

namespace Ui {
class Personnalize;
}

class Personnalize : public QWidget
{
    Q_OBJECT

public:
    explicit Personnalize(QWidget * parent = 0);
    ~Personnalize();

private:
    Ui::Personnalize * ui;
    QString fileChooserDialog(QString openFolder);

signals:
    void proxySettingSignal();

public slots:
    void changeLanguage();
    void proxySettingEvent();
    void fileChooserInstallEvent();
    void fileChooserDataEvent();
};

#endif // PERSONNALIZE_H

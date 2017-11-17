#ifndef PERSONNALIZE_H
#define PERSONNALIZE_H

#include <QWidget>

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

public slots:
    void changeLanguage();
};

#endif // PERSONNALIZE_H

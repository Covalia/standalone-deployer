#include "gui/forms/windowui.h"

#include "gui/forms/askpopupui.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "io/config.h"

#include <QtWidgets>

WindowUI::WindowUI(QWidget * _centralWidget, const QString & _appName, QWidget * _parent) :
    QMainWindow(_parent),
    m_closeActionText(QT_TR_NOOP("Close")),
    m_titleLabelText(QT_TR_NOOP("Installation of %1")),
    m_appName(_appName),
    m_closeAction(0),
    m_toolbar(0),
    m_iconLabel(0),
    m_titleLabel(0),
    m_comboBoxLanguage(0)
{
    setMinimumWidth(700);
    setMinimumHeight(450);
    setMaximumWidth(700);
    setMaximumHeight(450);

    setWindowTitle(tr("Standalone deployment"));

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    if (_centralWidget) {
        _centralWidget->setVisible(true);
        setCentralWidget(_centralWidget);
    }

    m_toolbar = new QToolBar(this);
    m_toolbar->layout()->setContentsMargins(0, 0, 0, 0);
    m_toolbar->layout()->setSpacing(0);

    m_toolbar->setIconSize(QSize(45, 45));
    m_toolbar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, m_toolbar);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName("iconLabel");
    m_iconLabel->setPixmap(QPixmap(":/images/logo_title.png"));
    m_titleLabel = new QLabel(tr_helper(m_titleLabelText).arg(m_appName), this);
    m_titleLabel->setObjectName("titleLabel");
    m_comboBoxLanguage = new QComboBox(this);
    connect(m_comboBoxLanguage, SIGNAL(activated(int)), this, SLOT(comboBoxLanguageEvent(int)));

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_comboBoxLanguage->sizePolicy().hasHeightForWidth());
    m_comboBoxLanguage->setSizePolicy(sizePolicy);

    m_comboBoxLanguage->addItem("Francais", QVariant(IOConfig::LocaleFrFr));
    m_comboBoxLanguage->addItem("English", QVariant(IOConfig::LocaleEnUs));
    m_comboBoxLanguage->setFocusPolicy(Qt::NoFocus);
    m_closeAction = new QAction(QIcon(":/images/close.png"), tr_helper(m_closeActionText), this);

    connect(m_closeAction, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));

    QWidget * spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_toolbar->addWidget(m_iconLabel);
    m_toolbar->addWidget(m_titleLabel);
    m_toolbar->addWidget(spacer);
    m_toolbar->addWidget(m_comboBoxLanguage);
    m_toolbar->addAction(m_closeAction);

    // windows title and icon
    retranslateUi();
}

WindowUI::~WindowUI()
{
    delete m_closeAction;
}

void WindowUI::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    m_position = center;
    move(geometry.topLeft());
}

void WindowUI::mousePressEvent(QMouseEvent * _e)
{
    m_position = QPoint(_e->globalX() - geometry().x(), _e->globalY() - geometry().y());
    setCursor(QCursor(Qt::SizeAllCursor));
}

void WindowUI::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(QCursor(Qt::ArrowCursor));
}

void WindowUI::mouseMoveEvent(QMouseEvent * _e)
{
    QWidget::mouseMoveEvent(_e);
    if (_e->buttons() && Qt::LeftButton) {
        QPoint toMove = _e->globalPos() - m_position;
        move(toMove);
        repaint();
    }
}

void WindowUI::closeEvent(QCloseEvent * _event)
{
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this, tr("Do you want to exit the application?"), tr("The installation will be stopped"));
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

void WindowUI::comboBoxLanguageEvent(int _index)
{
    const QString language = m_comboBoxLanguage->itemData(_index).toString();
    L_INFO("Detect language change in language combobox: " + language);
    if (!language.isNull() && !language.isEmpty()) {
        LanguageManager::updateLocale(language);
        emit changeLanguageSignal(language);
    }
}

void WindowUI::retranslateUi()
{
    const QString className = metaObject()->className();

    m_titleLabel->setText(translate_helper(className, m_titleLabelText).arg(m_appName));
    m_closeAction->setText(translate_helper(className, m_closeActionText));
}

void WindowUI::setLocale(const QString &_locale)
{
	L_INFO("Set combobox index for locale: " + _locale);
    const int index = m_comboBoxLanguage->findData(_locale);

    if (index != -1) {
        m_comboBoxLanguage->setCurrentIndex(index);
        comboBoxLanguageEvent(index);
    }
}

QString WindowUI::getLocale() const
{
    return m_comboBoxLanguage->currentData().toString();
}

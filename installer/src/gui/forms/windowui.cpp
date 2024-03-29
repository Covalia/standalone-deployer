#include "gui/forms/windowui.h"
#include "gui/forms/askpopupui.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "io/config.h"

#include <QtWidgets>
#include "installerfactories/osresources/osresources.h"

WindowUI::WindowUI(QWidget * _centralWidget, const QString & _appName, QWidget * _parent) :
    QMainWindow(_parent),
    m_closeActionText(QT_TR_NOOP("Close")),
    m_titleLabelText(QT_TR_NOOP("Installation of %1")),
    m_appName(_appName),
    m_closeAction(nullptr),
    m_toolbar(nullptr),
    m_iconLabel(nullptr),
    m_titleLabel(nullptr),
    m_comboBoxLanguage(nullptr)
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
        setCentralWidget(encloseByWidget(_centralWidget));
    }

    m_toolbar = new QToolBar(this);
    m_toolbar->layout()->setContentsMargins(0, 0, 0, 0);
    m_toolbar->layout()->setSpacing(0);
    // disable right clic to hide toolbar
    m_toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    m_toolbar->setIconSize(QSize(45, 45));
    m_toolbar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, m_toolbar);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName("iconLabel");

    // get image from resources to temp file
    QTemporaryFile titlePngFile;
    if (titlePngFile.open()) {
        if (OsResources::extractTitlePngToTempFile(titlePngFile.fileName())) {
            L_INFO("title.png extracted from application resources.");
            m_iconLabel->setPixmap(QPixmap(titlePngFile.fileName()));
        } else {
            L_ERROR("Unable to open title.png from application resources.");
        }
    } else {
        L_ERROR("Unable to open temporary file for title.png.");
    }

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
    spacer->setObjectName("spacer");
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

void WindowUI::closeEvent(QCloseEvent * _event)
{
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this);
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

    L_INFO(QString("Detect language change in language combobox: %1").arg(language));
    if (!language.isNull() && !language.isEmpty()) {
        LanguageManager::updateLocale(language);
        emit changeLanguageSignal(language);
    }
}

void WindowUI::retranslateUi()
{
    L_INFO("WindowUI::retranslateUi() called");

    const QString className = metaObject()->className();

    m_titleLabel->setText(translate_helper(className, m_titleLabelText).arg(m_appName));
    m_closeAction->setText(translate_helper(className, m_closeActionText));
}

void WindowUI::setLocale(const QString &_locale)
{
    L_INFO(QString("Set combobox index for locale: %1").arg(_locale));
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

void WindowUI::setCentralWidget(QWidget * _widget)
{
    QMainWindow::setCentralWidget(encloseByWidget(_widget));
}

QWidget * WindowUI::encloseByWidget(QWidget * _innerWidget)
{
    QWidget * outerWidget = new QWidget;

    outerWidget->setObjectName("outerWidget");

    QGridLayout * layout = new QGridLayout(outerWidget);
    layout->setContentsMargins(1, 1, 1, 1);
    outerWidget->setLayout(layout);

    layout->addWidget(_innerWidget);

    return outerWidget;
}

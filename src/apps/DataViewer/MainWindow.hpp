
#pragma once

#include <QMainWindow>
#include <QString>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QLineEdit;
class QToolBar;
QT_END_NAMESPACE

// Forward Declarations
class cCentralWidget;

class cSsnxModel_file;
class cSsnxView;

class cOusterModel_net;
class cOusterView;


namespace Ui 
{
    class MainWindow;
}

class cMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cMainWindow(QWidget* parent = nullptr);
    ~cMainWindow();

    void initialize();

public slots:
    void onStatusUpdate(QString msg);
    void onInfoMessage(QString title, QString msg);
    void onWarningMessage(QString title, QString msg);
    void onErrorMessage(QString title, QString msg);

private:
    void createMainMenu();
    void createSubMenusAndActions();
    void createActions();
    void createStatusBar();

    QMenu* mpFileMenu;

    Ui::MainWindow* mpUI;
    cCentralWidget* mpCentralWidget = nullptr;

    cSsnxModel_file* mpSsnxModel = nullptr;
    cSsnxView*       mpSsnxView = nullptr;

    cOusterView* mpOusterView;
};


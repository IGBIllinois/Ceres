
#pragma once

#include "CeresDataFile.hpp"

#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
    class QLabel;
    class QLineEdit;
    class QPushButton;
    class QGroupBox;
QT_END_NAMESPACE


// Forward Declarations


class cCentralWidget : public QWidget, public cCeresDataFile
{
    Q_OBJECT

public:
    explicit cCentralWidget(QWidget* parent = nullptr);
    ~cCentralWidget();

    void initialize();
    
signals:
    void statusMessage(QString msg);
    void errorMessage(QString title, QString msg);

private slots:
    void browseSourceFile();
    void loadSourceFile();

private:
    bool readHeaderData();

private:
    void onBeginHeader() override;
    void onEndOfHeader() override;

    void onBeginFooter() override;
    void onEndOfFooter() override;

    void onExperimentTitle(const std::string& title) override;
    void onExperimentCultivar(const std::string& cultivar) override;
    void onExperimentResearcher(const std::string& researcher) override;
    void onExperimentDoc(const std::string& doc) override;

private:
    bool mHasBeginHeader = false;
    bool mHeaderComplete = false;
    bool mHasBeginFooter = false;
    bool mFooterComplete = false;

    QLineEdit*   mpLoadSrcFile;
    QPushButton* mpLoadSrcButton;
    QPushButton* mpLoadButton;

    QGroupBox* mpExperimentInfo;
    QLabel*    mpTitleLabel;
    QLineEdit* mpTitle;
    QLabel*    mpCultivarLabel;
    QLineEdit* mpCultivar;
    QLabel*    mpResearcherLabel;
    QLineEdit* mpResearcher;
};


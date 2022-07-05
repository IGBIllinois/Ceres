
#pragma once

#include <QSplashScreen>

class cCeresSplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    cCeresSplashScreen();
    ~cCeresSplashScreen();

    void showMessage(const QString& message);

protected:
    void drawContents(QPainter* painter) override;

private:
    QPixmap* mpPixmap;
    QString  mMessage;
};


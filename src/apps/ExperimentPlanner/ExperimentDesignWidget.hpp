
#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QAbstractButton;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
QT_END_NAMESPACE


// Forward Declaration
class cExperimentFile;

class cExperimentDesignWidget : public QGraphicsView
{
public:
	explicit cExperimentDesignWidget(QWidget* parent = nullptr);

    void clear();
    void loadExperiment(const cExperimentFile& experiment);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	
public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);

protected:
//    void paintEvent(QPaintEvent *event) override;

private:
    QGraphicsScene mScene;
    QPen    mPen;
    QBrush  mBrush;
    bool    mAntialiased = false;
    bool    mTransformed = false;
};
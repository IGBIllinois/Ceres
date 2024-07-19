
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
    Q_OBJECT

public:
	explicit cExperimentDesignWidget(QWidget* parent = nullptr);

    void clear();
    void loadExperiment(const cExperimentFile& experiment);

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

signals:
    void clearPaths();
    void drawPath(int x1_mm, int y1_mm, int x2_mm, int y2_mm);
    void insertBefore(int id, int type);
    void insertAfter(int id, int type);
    void deleteStep(int id);

public slots:
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    void stepUpdated();

protected:
    void redrawPath(const cExperimentFile& experiment);
    void experimentChange(bool changed);

private:
    QGraphicsScene mScene;
    QPen    mPen;
    QBrush  mBrush;
    bool    mAntialiased = false;
    bool    mTransformed = false;
};
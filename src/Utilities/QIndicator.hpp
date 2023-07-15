
#pragma once

#include <QWidget>
#include <QStaticText>
#include <QTextOption>


class QIndicator : public QWidget
{
    Q_OBJECT

public:
    QIndicator(QWidget* parent = nullptr);
    QIndicator(const QString& text, QWidget* parent = nullptr);
	virtual ~QIndicator();

	void setText(const QString& text);
	QString text() const;

    bool getState() const;
    void setState(bool state);

    void toggle();

    void setOnColor(QColor onColor);
    void setOnPattern(Qt::BrushStyle onPattern);

    void setOffColor(QColor offColor);
    void setOffPattern(Qt::BrushStyle offPattern);

    void setLedSize(int size);

signals:
    void stateChanged(bool lit);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
	QStaticText mText;

    bool    mLit;

    QColor  mLedOnColor;
    Qt::BrushStyle mLedOnPattern;

    QColor  mLedOffColor;
    Qt::BrushStyle mLedOffPattern;

    int mLedSize;
};
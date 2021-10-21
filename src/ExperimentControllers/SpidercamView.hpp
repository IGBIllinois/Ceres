
#pragma once

#include "ExperimentCtrlView.hpp"

#include <QPainter>
#include <QPen>

class cSpidercamView : public cExperimentControlView
{
public:
	cSpidercamView();
	~cSpidercamView();

	QString getViewTitle() const override;

public slots:
	void onUpdatedPosition();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
    QPainter*	mpDrawingArea;
	QPen* mpBorderPen;
};


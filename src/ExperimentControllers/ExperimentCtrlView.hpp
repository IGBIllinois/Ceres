
#pragma once

#include <QWidget>


class cExperimentControlView : public QWidget
{
    Q_OBJECT

public:
	virtual ~cExperimentControlView() = default;

    /*
     * Returns the preferred window title for the corresponding view.
     */
    virtual QString getViewTitle() const = 0;

protected:
	cExperimentControlView() = default;
};
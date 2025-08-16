
#pragma once

#include <QObject>

class iReferenceMarker
{
public:
	virtual ~iReferenceMarker() {}

public slots:
    virtual void onStartingReferenceMeasurement() = 0;
    virtual void onEndingReferenceMeasurement() = 0;
};

Q_DECLARE_INTERFACE(iReferenceMarker, "iReferenceMarker")

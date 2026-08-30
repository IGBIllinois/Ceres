
#pragma once

#include <hyspex/datatypes.h>


#include <QMetaType>

// Expose the HySpex state enumerations to the Qt meta system to pass them using signal/slots
Q_DECLARE_METATYPE(hyspex::AcquisitionStatus);
Q_DECLARE_METATYPE(hyspex::BackgroundStatus);
Q_DECLARE_METATYPE(hyspex::CommunicationStatus);
Q_DECLARE_METATYPE(hyspex::CoolingStatus);
Q_DECLARE_METATYPE(hyspex::InitStatus);
Q_DECLARE_METATYPE(hyspex::RecordingStatus);
Q_DECLARE_METATYPE(hyspex::ShutterStatus);


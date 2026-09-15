
#include "IrCameraModel.hpp"

#include "IronbowColorTable.hpp"
#include "RainbowColorTable.hpp"
#include "Rainbow_HC_ColorTable.hpp"
#include "WhiteHotColorTable.hpp"
#include "BlackHotColorTable.hpp"
#include "ArcticColorTable.hpp"


namespace
{
	static uint8_t ir_camera_id = 0;
}


cIrCameraModel::cIrCameraModel(const std::string& name, QObject* parent)
:
	cSensorModel(name, parent), mDeviceID(++ir_camera_id)
{
	qRegisterMetaType<eColorTable>();

    mColorTable = std::make_unique<cIronbowColorTable>();
}

uint8_t cIrCameraModel::device_id() const
{
	return mDeviceID;
}

void cIrCameraModel::updateViews()
{
    if (mColorTable)
        emit colorModelUpdated(mColorTable->getType());
}

void cIrCameraModel::setColorModel(eColorTable color_table)
{
    switch (color_table)
    {
    case eColorTable::IRONBOW:
        mColorTable = std::make_unique<cIronbowColorTable>();
        break;
    case eColorTable::RAINBOW:
        mColorTable = std::make_unique<cRainbowColorTable>();
        break;
    case eColorTable::RAINBOW_HC:
        mColorTable = std::make_unique<cRainbow_HC_ColorTable>();
        break;
    case eColorTable::WHITE_HOT:
        mColorTable = std::make_unique<cWhiteHotColorTable>();
        break;
    case eColorTable::BLACK_HOT:
        mColorTable = std::make_unique<cBlackHotColorTable>();
        break;
    case eColorTable::ARCTIC:
        mColorTable = std::make_unique<cArcticColorTable>();
        break;
    default:
        return;
    }

	emit colorModelUpdated(color_table);
}




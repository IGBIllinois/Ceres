/*
 * The OusterPropertyPage to control the OUSTER LiDAR.
 */

#pragma once

#include "../SensorPropertyPage.hpp"


class cOusterPropertyPage : public cSensorPropertyPage
{
public:
    cOusterPropertyPage(QWidget* parent = nullptr);
    ~cOusterPropertyPage() = default;
};



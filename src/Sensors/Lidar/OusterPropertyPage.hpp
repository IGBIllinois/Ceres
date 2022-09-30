/*
 * The OusterPropertyPage to control the OUSTER LiDAR.
 */

#pragma once

#include "../SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"


class cOusterPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cOusterPropertyPage(QWidget* parent = nullptr);
    ~cOusterPropertyPage() = default;

public:
    cExperimentState* createState(const std::string& type) override;

protected:

};



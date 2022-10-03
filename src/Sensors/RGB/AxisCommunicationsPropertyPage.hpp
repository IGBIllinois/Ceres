/*
 * The AxisCommunicationsPropertyPage to control the Axis Communications WebCams.
 */

#pragma once

#include "../SensorPropertyPage.hpp"
#include "ExperimentStateCreator.hpp"


class cAxisCommunicationsPropertyPage : public cSensorPropertyPage, public cExperimentStateCreator
{
public:
    cAxisCommunicationsPropertyPage(QWidget* parent = nullptr);
    ~cAxisCommunicationsPropertyPage() = default;

    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type) override;

protected:

};



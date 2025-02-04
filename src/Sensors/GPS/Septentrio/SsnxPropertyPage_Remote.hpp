/*
 * The GpsPropertyPage to control a GPS unit
 * connected to a remote computer.
 */

#pragma once

#include "../GpsPropertyPage_Remote.hpp"

class cSsnxPropertyPage_Remote : public cGpsPropertyPage_Remote
{
    Q_OBJECT

public:
    cSsnxPropertyPage_Remote(QWidget* parent = nullptr);
    ~cSsnxPropertyPage_Remote() = default;

    void doLayout() override;
};



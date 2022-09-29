/*
 * The OusterPropertyPage to control the OUSTER LiDAR
 * connected to a remote computer.
 */

#pragma once

#include "OusterPropertyPage.hpp"
#include "OusterPropertiesNetDecoder.hpp"
#include "OusterPropertiesNetEncoder.hpp"


class cOusterPropertyPage_Remote : public cOusterPropertyPage,
    private cOusterPropertiesNetDecoder, private cOusterPropertiesNetEncoder
{

public:
    cOusterPropertyPage_Remote(QWidget* parent = nullptr);
    ~cOusterPropertyPage_Remote() = default;
};



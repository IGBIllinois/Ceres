/*
 * The SsnxPropertyPage to control a GPS unit
 * connected to a remote computer.
 */

#pragma once

#include "../GpsPropertyPage_Remote.hpp"

class cSsnxPropertyPage_Remote : public cGpsPropertyPage_Remote
{
    Q_OBJECT

public:
    cSsnxPropertyPage_Remote(QWidget* parent = nullptr);
    explicit cSsnxPropertyPage_Remote(const std::string& instance, QWidget* parent = nullptr);
    ~cSsnxPropertyPage_Remote() = default;

public:
    cExperimentState* createState(const std::string& type, const nlohmann::json& entry, QObject* parent) override;
};



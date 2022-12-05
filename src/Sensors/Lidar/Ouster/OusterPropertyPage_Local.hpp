/*
 * The OusterPropertyPage to control the OUSTER LiDAR.
 */

#pragma once

#include "OusterPropertyPage.hpp"

class cOusterModel;

class cOusterPropertyPage_Local : public cOusterPropertyPage
{
    Q_OBJECT

public:
    cOusterPropertyPage_Local(cOusterModel* model, QWidget* parent = nullptr);
    ~cOusterPropertyPage_Local() = default;

    void doLayout() override;

signals:
    void requestNewAzimuthWindow(double min_deg, double max_deg);
    //    void requestNewLidarMode(ouster::eLIDAR_MODE mode);
    void requestNewLidarMode(QString mode);

public slots:
    void azimuthWindowChanged();
    void dataFormatChanged();

protected:
    void showPage() override;
    void doOK() override;
    void doCancel() override;
    void doApply() override;

private:
    cOusterModel* mpModel = nullptr;
};



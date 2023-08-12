
#pragma once

#include "HySpexCamera_Model.hpp"

#include <cbdf/HySpexVNIR_3000N_Serializer.hpp>

#include <QObject>


class cHySpexVNIR_3000N_Model : public cHySpexCameraModel
{
    Q_OBJECT

public:
    cHySpexVNIR_3000N_Model(QObject* parent = nullptr);
    virtual ~cHySpexVNIR_3000N_Model();

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override;
    void disableDataRecording() override;

    void writeDataHeader() override;

    virtual void open_shutter() = 0;
    virtual void close_shutter() = 0;

signals:
    void backgroundComplete();

protected:
    cHySpexVNIR_3000N_Serializer mSerializer;
};


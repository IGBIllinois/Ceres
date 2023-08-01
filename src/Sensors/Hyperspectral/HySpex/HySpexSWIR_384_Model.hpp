
#pragma once

#include "HySpexCamera_Model.hpp"

#include <cbdf/HySpexSWIR_384_Serializer.hpp>

#include <QObject>

class cHySpexSWIR_384_Model : public cHySpexCameraModel
{
    Q_OBJECT

public:
    cHySpexSWIR_384_Model(QObject* parent = nullptr);
    virtual ~cHySpexSWIR_384_Model() = default;

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

private:
    cHySpexSWIR_384_Serializer mSerializer;
};



#pragma once

#include "SsnxModel.hpp"
#include "SsnxParser.hpp"

#include <QObject>

class cSsnxModel_file : public cSsnxModel, public cSsnxParser
{
    Q_OBJECT

public:
    explicit cSsnxModel_file(QObject* parent = nullptr);
    ~cSsnxModel_file();

    bool configure(const nlohmann::json& jsonCfg) override;

    void enableDataRecording(cBlockDataFileWriter& file) override {};
    void disableDataRecording() override {};

    void writeDataHeader() override {};

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

protected:
    void update() override;
};


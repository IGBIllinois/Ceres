
#pragma once

#include "../RgbTypes.hpp"
#include "../RgbCameraModel.hpp"



class cLucidVisionLabsRgbModel : public cRgbCameraModel
{
    Q_OBJECT

public:
    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

    /*
     * Returns a string used as a data descriptor of the
     * type sensor.
     */
    static const char* data_type() { return "rgb"; };

    /*
     * Returns a string used as a protocol descriptor of the
     * type sensor.
     */
    static const char* protocol() { return "http"; };

    /*
     * Returns the class identifier used by the sensor's serializer
     */
    uint16_t data_class_id() const override;

    bool isConnected() const { return mConnected; }

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void update() override;

signals:
    void onNewImage(const QImage& image);
    void cameraIdChanged(int id);
    void frameRateChanged(int rate_fps);
    void imageSizeChanged(int width, int height);

public slots:
    void requestImage();

protected slots:
 
protected:
    cLucidVisionLabsRgbModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cLucidVisionLabsRgbModel();

protected:
    bool mConnected = false;
};



#pragma once

//#include "../RgbTypes.hpp"
#include "../IrCameraModel.hpp"
//#include "AxisCommunicationsUtils.hpp"

//#include <cbdf/ImageBuffers.hpp>
#include <cbdf/TeledyneFlirSerializer.hpp>

#include <QNetworkReply>
#include <QUrl>
#include <QBitmap>

#include <vector>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE


class cTeledyneFlirCameraModel : public cIrCameraModel
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
    static const char* data_type() { return "ir"; };

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

protected slots:

protected:
    cTeledyneFlirCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cTeledyneFlirCameraModel();

protected:
    bool mConnected;

    QImage mCurrentImage;

    cTeledyneFlirSerializer mSerializer;
};


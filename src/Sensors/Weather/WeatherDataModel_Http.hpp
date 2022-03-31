
#pragma once

#include "WeatherDataModel.hpp"
#include "../../Utilities/Timers.hpp"

#include <QNetworkReply>
#include <QUrl>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE


class cWeatherDataModel_Http : public cWeatherDataModel
{
    Q_OBJECT

public:
    /*
     * Returns a string used as a protocol descriptor of the
     * type sensor.
     */
    static char* protocol() { return "http"; };

    QUrl url() const { return mUrl; }

    bool configure(const nlohmann::json& jsonCfg) override;

    /*
     * Starts/Stops communication with the endpoint.
     * These methods are called inside the QThread so that
     * all of the communication happens within the same thread!
     */
    bool startCommunications() override;
    void stopCommunications() override;

    void update() override;

protected slots:
    void requestReceived(QNetworkReply* pReply);

protected:
    cWeatherDataModel_Http(QObject* parent = nullptr);
    virtual ~cWeatherDataModel_Http();

    void getRequest();

    virtual void processReply(const std::string& reply) = 0;

private:
    QNetworkAccessManager* mpHttpManager;
    QUrl mUrl;

    cIntervalTimer	mTimer;
};


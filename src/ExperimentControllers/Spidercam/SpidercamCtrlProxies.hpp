
#pragma once

#include <spidercam_connect/spidercam_com.hpp>

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

class iSpidercamController
{
public:
	iSpidercamController() = default;

	virtual bool linkError() const = 0;

	virtual bool isBusy() const = 0;
	virtual bool isConsoleConnected() const = 0;
	virtual bool isMoving() const = 0;
	virtual bool isSetPointEnabled() const = 0;
	virtual bool isInScriptMode() const = 0;
	virtual bool isInError() const = 0;

	virtual const spidercam::sPosition_1_t& getLastKnownPosition() const = 0;

	virtual bool requestStop() = 0;

	virtual bool sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
		uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg) = 0;

protected:
	~iSpidercamController() = default;
};

template<class MODEL>
class cSpidercamCtrlProxy : public iSpidercamController
{
public:
	cSpidercamCtrlProxy(MODEL* pModel) : mpModel(pModel) {};
	~cSpidercamCtrlProxy() = default;

	bool linkError() const override;

	bool isBusy() const override;
	bool isConsoleConnected() const override;
	bool isMoving() const override;
	bool isSetPointEnabled() const override;
	bool isInScriptMode() const override;
	bool isInError() const override;

	const spidercam::sPosition_1_t& getLastKnownPosition() const override;

	bool requestStop() override;

	bool sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
		uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg) override;

private:
	MODEL* mpModel = nullptr;
};


template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::linkError() const
{
	return mpModel->linkError();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isBusy() const
{
	return mpModel->isBusy();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isConsoleConnected() const
{
	return mpModel->isConsoleConnected();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isMoving() const
{
	return mpModel->isMoving();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isSetPointEnabled() const
{
	return mpModel->isSetPointEnabled();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isInScriptMode() const
{
	return mpModel->isInScriptMode();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::isInError() const
{
	return mpModel->isInError();
};

template<class MODEL>
inline const spidercam::sPosition_1_t& cSpidercamCtrlProxy<MODEL>::getLastKnownPosition() const
{
	return mpModel->getLastKnownPosition();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::requestStop()
{
	return mpModel->requestStop();
};

template<class MODEL>
inline bool cSpidercamCtrlProxy<MODEL>::sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
	uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg)
{
	return mpModel->sendRequestNewPosition(x_mm, y_mm, z_mm, height_mm, speed_mmps, pan_deg, tilt_deg, roll_deg);
};

